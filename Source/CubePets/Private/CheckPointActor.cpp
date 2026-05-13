// Fill out your copyright notice in the Description page of Project Settings.


#include "CheckPointActor.h"
#include "Components/BoxComponent.h"
#include "CubePetsCharacter.h"
#include "CubePetsSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "SaveGameInstanceSubsystem.h"

// Sets default values
ACheckPointActor::ACheckPointActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	UE_LOG(LogTemp, Warning, TEXT("Overlap!"));

	// ÉZÅ[Éuèàóù
	if (Cast<ACubePetsCharacter>(OtherActor))
	{
		USaveGameInstanceSubsystem* SaveSubsystem = GetGameInstance()->GetSubsystem<USaveGameInstanceSubsystem>();
		SaveSubsystem->SetCurrentCheckPoint(GetActorLocation());

		UCubePetsSaveGame* SaveGame = Cast<UCubePetsSaveGame>(UGameplayStatics::CreateSaveGameObject(UCubePetsSaveGame::StaticClass()));
		SaveGame->SetCheckPointLocation(SaveSubsystem->GetCurrentCheckPoint());
		UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("Slot1"), 0);
	}
}

