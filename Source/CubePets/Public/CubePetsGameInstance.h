// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "CubePetsGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API UCubePetsGameInstance : public UGameInstance
{
	GENERATED_BODY()

protected:

	FVector mCurrentCheckPoint;

public:

	UFUNCTION(BlueprintCallable, Category="CheckPoint")
	void SetCurrentCheckPoint(FVector NewLocation)
	{
		mCurrentCheckPoint = NewLocation;
	};

	UFUNCTION(BlueprintPure, Category="CheckPoint")
	FVector GetCurrentCheckPoint() const
	{
		return mCurrentCheckPoint;
	}

public:

	void SaveToDisk();
	void LoadFromDisk();
	
};
