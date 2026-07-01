// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CheckPointSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API UCheckPointSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:

	FVector mCurrentCheckPoint;
	bool bHasCheckPoint = false;

public:

	UFUNCTION()
	bool HasCheckPoint() const
	{
		return bHasCheckPoint;
	}

public:

	UFUNCTION(BlueprintCallable, Category = "CheckPoint")
	void SetCurrentCheckPoint(FVector NewLocation)
	{
		mCurrentCheckPoint = NewLocation;
		bHasCheckPoint = true;
	};

	UFUNCTION(BlueprintPure, Category = "CheckPoint")
	FVector GetCurrentCheckPoint() const
	{
		return mCurrentCheckPoint;
	}

public:

	UFUNCTION()
	void ResetCheckPoint()
	{
		mCurrentCheckPoint = FVector::ZeroVector;
		bHasCheckPoint = false;
	}
	
};
