// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/PickupItemBase.h"
#include "Components/BoxComponent.h"

// Sets default values
APickupItemBase::APickupItemBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	RootComponent = mBoxTrigger;

	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	mStaticMesh->SetupAttachment(mBoxTrigger);

}

// Called when the game starts or when spawned
void APickupItemBase::BeginPlay()
{
	Super::BeginPlay();

	if (mBoxTrigger)
	{
		mBoxTrigger->OnComponentBeginOverlap.AddDynamic(this, &APickupItemBase::OnOverlapBegin);
	}
}

// Called every frame
void APickupItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickupItemBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool FromSweep,
	const FHitResult& SweepResult
)
{
}

