// Fill out your copyright notice in the Description page of Project Settings.


#include "CanonBase.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"

ACanonBase::ACanonBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// StaticMesh
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = mStaticMesh;

	// BoxTrigger
	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	mBoxTrigger->SetupAttachment(mStaticMesh);
	mBoxTrigger->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	mBoxTrigger->SetCollisionProfileName(TEXT("Trigger"));
}

void ACanonBase::BeginPlay()
{
	Super::BeginPlay();

	// 0.2秒ごとに関数を実行
	GetWorldTimerManager().SetTimer(mSearchTimerHandle, this, &ACanonBase::SearchForPlayer, 0.2f, true);
}

void ACanonBase::SearchForPlayer()
{
	if (!mBoxTrigger) return;

	// 判定に使うboxの形状を作成 (BoxTriggerのスケールに合わせる)
	FCollisionShape BoxShape = FCollisionShape::MakeBox(mBoxTrigger->GetScaledBoxExtent());

	// 判定のクエリパラメータ設定
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 自分自身は無視

	// 検索対象をPawnのみに限定する設定
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	// Overlapテストを実行
	bool bIsOverlapping = GetWorld()->OverlapAnyTestByObjectType
	(
		mBoxTrigger->GetComponentLocation(), // 位置
		mBoxTrigger->GetComponentQuat(), // 回転
		ObjectParams, // オブジェクトタイプ
		BoxShape, // 形状
		Params
	);

	if (bIsOverlapping)
	{
		OnPlayerDetected();
	}
}


