// Fill out your copyright notice in the Description page of Project Settings.


#include "ConveyorVolume.h"
#include "Components/BoxComponent.h"

// Sets default values
AConveyorVolume::AConveyorVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mTriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	RootComponent = mTriggerBox;

	mConveyorVelocity = FVector(20.0f, 0.0f, 0.0f);
}

// Called when the game starts or when spawned
void AConveyorVolume::BeginPlay()
{
	Super::BeginPlay();
	
	mTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AConveyorVolume::OnOverlapBegin);
	mTriggerBox->OnComponentEndOverlap.AddDynamic(this, &AConveyorVolume::OnOverlapEnd);
}

// Called every frame
void AConveyorVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 配列が空なら何もしない
	if (mOverlappingActors.IsEmpty()) return;

	FVector DeltaLocation = mConveyorVelocity * DeltaTime;

	// リスト内のすべてのアクターを動かす
	for (AActor* ActorToMove : mOverlappingActors)
	{
		if (ActorToMove && ActorToMove->IsValidLowLevel())
		{
			// 箱は物理オブジェクトなので速度を上書きする形で動かす
			UPrimitiveComponent* PrimitiveComp = Cast<UPrimitiveComponent>(ActorToMove->GetRootComponent());

			if (PrimitiveComp)
			{
				FVector CurrentVelocity = PrimitiveComp->GetPhysicsLinearVelocity();
				FVector NewVelocity = FVector(mConveyorVelocity.X, mConveyorVelocity.Y, CurrentVelocity.Z);
				PrimitiveComp->SetPhysicsLinearVelocity(NewVelocity);
			}
		}
	}

}

void AConveyorVolume::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// Z軸で回転しないようにする
	if (OtherComp)
	{
		FBodyInstance* BodyInst = OtherComp->GetBodyInstance();
		if (BodyInst)
		{
			BodyInst->bLockZRotation = true;
			BodyInst->SetDOFLock(EDOFMode::SixDOF);
		}
	}

	// 触れたアクターを配列に追加（自分は除く）
	if (OtherActor && OtherActor != this)
	{
		mOverlappingActors.AddUnique(OtherActor);
	}
}

void AConveyorVolume::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	// Z軸回転のロックを解除する
	FBodyInstance* BodyInst = OtherComp->GetBodyInstance();
	if (BodyInst)
	{
		BodyInst->bLockZRotation = false;
		BodyInst->SetDOFLock(EDOFMode::SixDOF);
	}

	// 領域から出たアクターを配列から除外
	if (OtherActor)
	{
		mOverlappingActors.Remove(OtherActor);
	}
}

