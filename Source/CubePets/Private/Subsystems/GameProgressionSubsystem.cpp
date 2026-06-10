// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameProgressionSubsystem.h"

void UGameProgressionSubsystem::UnLockNextStage(int32 ClearedStageIndex)
{
	if (ClearedStageIndex == mMaxUnlockedStageIndex)
	{
		mMaxUnlockedStageIndex++;
	}
}
