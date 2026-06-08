// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmicks/CanonBase.h"
#include "TimerManager.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

ACanonBase::ACanonBase()
{
	PrimaryActorTick.bCanEverTick = true;

	// MeshBase
	mMeshBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBase"));
	RootComponent = mMeshBase;

	// MeshTurret
	mMeshTurret = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshTurret"));
	mMeshTurret->SetupAttachment(mMeshBase);

	// BoxTrigger
	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	mBoxTrigger->SetupAttachment(mMeshBase);
	mBoxTrigger->SetCollisionProfileName(TEXT("Trigger"));
	mBoxTrigger->SetUsingAbsoluteScale(true);
	mBoxTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
}

void ACanonBase::BeginPlay()
{
	Super::BeginPlay();

	// 0.2秒ごとにプレイヤーが射程圏内にいるか探知する
	GetWorldTimerManager().SetTimer(mSearchTimerHandle, this, &ACanonBase::SearchForPlayer, 0.2f, true);
}

void ACanonBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn && mMeshTurret)
	{
		// 自分とプレイヤーの現在位置を取得
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = PlayerPawn->GetActorLocation();

		// プレイヤーへ向かう方向を計算（長さを1に正規化）
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

		// プレイヤーのほうを向かせる（砲台だけ）
		FRotator CompleteRotation = Direction.Rotation();
		FRotator NewRotation = FRotator(0.0f, CompleteRotation.Yaw, 0.0f);

		mMeshTurret->SetWorldRotation(NewRotation);
	}
}

FTransform ACanonBase::GetBulletSpawnPointTransform() const
{
	return BP_GetBulletSpawnPointTransform();
}

void ACanonBase::SearchForPlayer()
{
	if (!mBoxTrigger) return;

	// 判定に使うboxの形状を作成 (BoxTriggerのスケールに合わせる)
	FCollisionShape BoxShape = FCollisionShape::MakeBox(mBoxTrigger->GetScaledBoxExtent());

	// 判定のクエリパラメータ設定
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 自分自身は無視

	// 検索対象をPawnのみに限定する
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_Pawn);

	bool bIsOverlapping = GetWorld()->OverlapAnyTestByObjectType
	(
		mBoxTrigger->GetComponentLocation(),
		mBoxTrigger->GetComponentQuat(),
		ObjectParams,
		BoxShape,
		Params
	);

	if (bIsOverlapping)
	{
		OnPlayerDetected();
	}
}


