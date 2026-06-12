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

const FStageData* UGameProgressionSubsystem::GetStageDataByIndex(int32 Index) const
{
	if (!mStageDataTable) return nullptr;

	TArray<FStageData*> AllRows;
	mStageDataTable->GetAllRows<FStageData>(TEXT(""), AllRows);

	if (AllRows.IsValidIndex(Index))
	{
		return AllRows[Index];
	}
	return nullptr;
}

void UGameProgressionSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// データテーブルを取得して、メダル総数と箱使用目標数の配列を初期化する
	const UCubePetsGameSettings* Settings = GetDefault<UCubePetsGameSettings>();
	if (Settings && Settings->mMedalDataTablePath.IsValid())
	{
		mStageDataTable = Cast<UDataTable>(Settings->mMedalDataTablePath.TryLoad());
	}

	if (mStageDataTable)
	{
		int32 NumStages = mStageDataTable->GetRowNames().Num();
		mCurrentMedalCountArray.Init(0, NumStages);
		mCurrentUsedCubeCountArray.Init(0, NumStages);
	}

}
