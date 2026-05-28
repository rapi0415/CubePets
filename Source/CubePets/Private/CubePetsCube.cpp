// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePetsCube.h"

// Sets default values
ACubePetsCube::ACubePetsCube()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = mStaticMesh;
	mStaticMesh->SetSimulatePhysics(false);
}

// Called when the game starts or when spawned
void ACubePetsCube::BeginPlay()
{
	Super::BeginPlay();

	if (mStaticMesh)
	{
		mDynamicMaterial = mStaticMesh->CreateAndSetMaterialInstanceDynamic(0);
	}
	
}

// Called every frame
void ACubePetsCube::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// 落下しても消えずに非アクティブにするだけにする
void ACubePetsCube::FellOutOfWorld(const UDamageType& dmgType)
{
	OnDeactivated();
}

// 存在していない状態にする
void ACubePetsCube::OnDeactivated()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);


	mStaticMesh->SetSimulatePhysics(false);

	bIsActive = false;
}

// 存在している状態にする
void ACubePetsCube::OnActivated(FVector SpawnLocation, FRotator SpawnRotation)
{
	SetActorLocation(SpawnLocation, false, nullptr, ETeleportType::TeleportPhysics); // テレポートで移動させる
	SetActorRotation(SpawnRotation);

	// 速度を0にしておく
	UPrimitiveComponent* RootComp = Cast<UPrimitiveComponent>(GetRootComponent());
	if (RootComp)
	{
		RootComp->SetAllPhysicsLinearVelocity(FVector::ZeroVector);
	}

	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);

	// 物理をON
	if (mStaticMesh)
	{
		mStaticMesh->SetSimulatePhysics(true);
	}

	bIsActive = true;
}

void ACubePetsCube::SetGlow(bool bIsOldest)
{
	if (mDynamicMaterial)
	{
		if (bIsOldest)
		{
			// 光らせる
			mDynamicMaterial->SetScalarParameterValue(TEXT("GlowIntensity"), 1.0f);
		}
		else
		{
			// 元に戻す
			mDynamicMaterial->SetScalarParameterValue(TEXT("GlowIntensity"), 0.0f);
		}
	}
}


