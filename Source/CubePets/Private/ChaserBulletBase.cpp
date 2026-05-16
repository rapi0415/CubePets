// Fill out your copyright notice in the Description page of Project Settings.


#include "ChaserBulletBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"

// Sets default values
AChaserBulletBase::AChaserBulletBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChaserBulletBase::BeginPlay()
{
	Super::BeginPlay();

	// スフィアコリジョン
	mSphereTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("SpehreTrigger"));
	RootComponent = mSphereTrigger;

	// スタティックメッシュ
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	mStaticMesh->SetupAttachment(RootComponent);
}

// Called every frame
void AChaserBulletBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	if (PlayerPawn)
	{
		// 自分とプレイヤーの現在位置を取得
		FVector CurrentLocation = GetActorLocation();
		FVector TargetLocation = PlayerPawn->GetActorLocation();

		// プレイヤーへ向かう方向を計算（長さを1に正規化）
		FVector Direction = (TargetLocation - CurrentLocation).GetSafeNormal();

		// 移動後の新しい位置を計算
		FVector NewLocation = CurrentLocation + (Direction * mMoveSpeed * DeltaTime);

		SetActorLocation(NewLocation);

		// プレイヤーのほうを向かせる
		FRotator NewRotation = Direction.Rotation();
		SetActorRotation(NewRotation);
	}

}

