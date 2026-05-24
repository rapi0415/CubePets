// Fill out your copyright notice in the Description page of Project Settings.


#include "SwitchBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ASwitchBase::ASwitchBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// スイッチの見た目
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = mStaticMesh;

	// BoxTrigger
	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	mBoxTrigger->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASwitchBase::BeginPlay()
{
	Super::BeginPlay();

	// BoxTriggerが何かに触れたらOnOverlapBeginを呼ぶようにバインド（同様にOverlapEndも）
	if (mBoxTrigger)
	{
		mBoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &ASwitchBase::OnOverlapBegin);
		mBoxTrigger->OnComponentEndOverlap.AddDynamic(this, &ASwitchBase::OnOverlapEnd);
	}

	// 動的マテリアルを作成
	if (mStaticMesh)
	{
		mDynamicMaterial = mStaticMesh->CreateDynamicMaterialInstance(0);
	}
	
}

// Called every frame
void ASwitchBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitchBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	// スイッチ起動、処理はBPで定義
	if (OtherActor && OtherActor != this)
	{
		if (mTargetActor)
		{
			OnSwitchActivated(OtherActor, mTargetActor);
		}
	}

	// 色を変える
	if (mDynamicMaterial)
	{
		mDynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Red);
	}

}

void ASwitchBase::OnOverlapEnd(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	// スイッチ解除、処理はBPで定義
	if (OtherActor && OtherActor != this)
	{
		if (mTargetActor)
		{
			OnSwitchDeactivated(OtherActor, mTargetActor);
		}
	}

	// 色を変える
	if (mDynamicMaterial)
	{
		FLinearColor DefaultColor = FLinearColor(0.4f, 0.4f, 0.4f, 0.0f);
		mDynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), DefaultColor);
	}
}

