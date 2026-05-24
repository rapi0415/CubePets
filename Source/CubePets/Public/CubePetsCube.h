// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubePetsCube.generated.h"

class UMaterialInstanceDynamic;

UCLASS()
class CUBEPETS_API ACubePetsCube : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubePetsCube();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> mStaticMesh;

public:

	UFUNCTION()
	void OnDeactivated();

	UFUNCTION()
	void OnActivated(FVector SpawnLocation, FRotator SpawnRotation);

protected:

	UPROPERTY()
	bool bIsActive = false;

public:

	UFUNCTION()
	bool GetIsActive()
	{
		return bIsActive;
	}

public:

	UFUNCTION()
	void SetGlow(bool bIsOldest);

protected:

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> mDynamicMaterial;

};
