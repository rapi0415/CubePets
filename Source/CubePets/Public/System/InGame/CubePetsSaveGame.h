// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Enums/StageClearState.h"
#include "CubePetsSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API UCubePetsSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	UPROPERTY()
	int32 mMaxUnlockedStageIndex = 0;

	UPROPERTY()
	int32 mCurrentStageIndex = 0;

	UPROPERTY()
	TMap<FName, bool> mCollectedMedalMap;

	UPROPERTY()
	TArray<EStageClearState> mStageClearStates;

	UPROPERTY()
	TArray<int32> mCurrentMedalCountArray;

	UPROPERTY()
	TArray<int32> mCurrentUsedCubeCountArray;

	UPROPERTY()
	TArray<int32> mRecordCubeCountArray;


};
