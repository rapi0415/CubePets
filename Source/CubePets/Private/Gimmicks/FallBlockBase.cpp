// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmicks/FallBlockBase.h"
#include "Components/BoxComponent.h"

// Sets default values
AFallBlockBase::AFallBlockBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// スタティックメッシュ
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = mStaticMesh;

	// ボックストリガー
	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	mBoxTrigger->SetupAttachment(mStaticMesh);
	mBoxTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));

}

// Called when the game starts or when spawned
void AFallBlockBase::BeginPlay()
{
	Super::BeginPlay();

	if (mBoxTrigger)
	{
		mBoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &AFallBlockBase::OnOverlapBegin);
	}

	if (mStaticMesh)
	{
		mDynamicMaterial = mStaticMesh->CreateAndSetMaterialInstanceDynamic(0);
	}

}

// Called every frame
void AFallBlockBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFallBlockBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor && OtherActor != this)
	{
		// 光らせる
		SetGlow();

		// 一定時間後に落ちる
		OnFallBegin();
	}
}

void AFallBlockBase::SetGlow()
{
	if (mDynamicMaterial)
	{
		// 光らせる
		mDynamicMaterial->SetScalarParameterValue(TEXT("GlowIntensity"), 1.0f);
	}
}

void AFallBlockBase::OnFallBegin()
{
	BP_OnFallBegin();
}
