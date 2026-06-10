// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameProgressionSubsystem.h"
#include "System/StageSelect/CubePetsGameSettings.h"

void UGameProgressionSubsystem::UnLockNextStage(int32 ClearedStageIndex)
{
	if (ClearedStageIndex == mMaxUnlockedStageIndex)
	{
		mMaxUnlockedStageIndex++;
	}
}

const FMedalData* UGameProgressionSubsystem::GetMedalDataByIndex(int32 Index) const
{
	if (!mMedalDataTable) return nullptr;

	TArray<FMedalData*> AllRows;
	mMedalDataTable->GetAllRows<FMedalData>(TEXT(""), AllRows);

	if (AllRows.IsValidIndex(Index))
	{
		return AllRows[Index];
	}
	return nullptr;
}

void UGameProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	const UCubePetsGameSettings* Settings = GetDefault<UCubePetsGameSettings>();
	if (Settings && Settings->mMedalDataTablePath.IsValid())
	{
		mMedalDataTable = Cast<UDataTable>(Settings->mMedalDataTablePath.TryLoad());
	}

	if (mMedalDataTable)
	{
		int32 NumStages = mMedalDataTable->GetRowNames().Num();
		mCurrentMedalCountArray.Init(0, NumStages);
	}

}
