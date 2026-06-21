// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmicks/CheckPointActor.h"
#include "Components/BoxComponent.h"
#include "Characters/CubePetsCharacter.h"
#include "Subsystems/CheckPointSubsystem.h"

// Sets default values
ACheckPointActor::ACheckPointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// BoxTrigger
	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	RootComponent = mBoxTrigger;
	mBoxTrigger->SetBoxExtent(FVector(100.0f, 100.0f, 100.0f));
	mBoxTrigger->SetCollisionProfileName(TEXT("Trigger"));

	// StaticMesh
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	mStaticMesh->SetupAttachment(mBoxTrigger);
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

}

// Called when the game starts or when spawned
void ACheckPointActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACheckPointActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACheckPointActor::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	// 接触したのがプレイヤーならチェックポイントの座標をセーブする
	
	if (ACubePetsCharacter* PlayerCharacter = Cast<ACubePetsCharacter>(OtherActor))
	{
		// Subsystemの座標情報を更新
		UGameInstance* GameInstance = GetGameInstance();
		if (GameInstance)
		{
			UCheckPointSubsystem* Subsystem = GameInstance->GetSubsystem<UCheckPointSubsystem>();
			if (Subsystem)
			{
				Subsystem->SetCurrentCheckPoint(GetActorLocation());
			}
		}

		// エフェクト類を表示
		PlayEffects();

		// プレイヤーの頭上にセーブしたことを示すテキストを表示
		PlayerCharacter->ShowFloatingText();
	}
	
}

void ACheckPointActor::PlayEffects()
{
	BP_PlayEffects();
}

