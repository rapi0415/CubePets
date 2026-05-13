// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "CubePetsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API UCubePetsSaveGame : public USaveGame
{
	GENERATED_BODY()

protected:

	UPROPERTY()
	FVector mCheckPointLocation;

	UPROPERTY()
	FName mLevelName;

public:

	UFUNCTION(BlueprintCallable, Category="CheckPoint")
	void SetCheckPointLocation(FVector NewLocation)
	{
		mCheckPointLocation = NewLocation;
	};
	
	UFUNCTION(BlueprintPure, Category="CheckPoint")
	FVector GetCheckPointLocation() const
	{
		return mCheckPointLocation;
	}

};
