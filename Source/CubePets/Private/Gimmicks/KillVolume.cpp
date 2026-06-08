// Fill out your copyright notice in the Description page of Project Settings.


#include "Gimmicks/KillVolume.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Gimmicks/ConveyorVolume.h"

// Sets default values
AKillVolume::AKillVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = mBoxComponent;
}

// Called when the game starts or when spawned
void AKillVolume::BeginPlay()
{
	Super::BeginPlay();
	
	// Overlapイベントをバインド
	if (mBoxComponent)
	{
		mBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillVolume::OnOverlapBegin);
	}
}

// Called every frame
void AKillVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKillVolume::OnOverlapBegin
(
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
		if (ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor))
		{
			UGameplayStatics::ApplyDamage(OtherActor, 1.0f, nullptr, this, UDamageType::StaticClass());

			PlayEffects(OtherActor);
		}
	}
}

void AKillVolume::PlayEffects(AActor* TargetActor)
{
	BP_PlayEffects(TargetActor);
}

