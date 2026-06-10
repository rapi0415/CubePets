// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GameProgressionSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API UGameProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, Category="Stage")
	int32 mMaxUnlockedStageIndex = 0;

public:

	UFUNCTION(BlueprintCallable, Category="Stage")
	int32 GetMaxUnlockedStageIndex()
	{
		return mMaxUnlockedStageIndex;
	}

	UFUNCTION(BlueprintCallable, Category="Stage")
	void UnLockNextStage(int32 ClearedStageIndex);

};
