// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "KillVolume.generated.h"

class UBoxComponent;

UCLASS()
class CUBEPETS_API AKillVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AKillVolume();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxComponent> mBoxComponent;

	UFUNCTION()
	void OnOverlapBegin
	(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

protected:

	void PlayEffects(AActor* TargetActor);

	UFUNCTION(BlueprintImplementableEvent, Category="Effects")
	void BP_PlayEffects(AActor* TargetActor);

};
