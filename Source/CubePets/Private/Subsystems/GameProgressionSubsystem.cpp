// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/GameProgressionSubsystem.h"
#include "System/StageSelect/CubePetsGameSettings.h"
#include "System/InGame/CubePetsSaveGame.h"
#include "Kismet/GameplayStatics.h"

const FString UGameProgressionSubsystem::mSaveSlotName = TEXT("PlayerProgress");

void UGameProgressionSubsystem::ResetProgress()
{
	mMaxUnlockedStageIndex = 0; // 解放済みステージ数
	mCurrentStageIndex = 0; // 現在のステージ番号
	mCollectedMedalMap.Empty(); // メダルの獲得情報
	mCollectedMedalMapTemp.Empty(); // メダルの獲得情報（一時保存用）
	mCheckPointCubeCount = 0; // 使った箱の数（チェックポイント保存用）

	// データテーブルを取得して、配列を初期化する
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
		mRecordCubeCountArray.Init(100, NumStages); // 最小値を記録していきたいので初期値は大きい値にする
		mStageClearStates.Init(EStageClearState::NotCleared, NumStages);
	}
}

void UGameProgressionSubsystem::UpdateCurrentStageClearState(int32 Index)
{
	// 呼ばれた時点でそのステージはクリア済みと判定
	// 集めたメダルと使った箱を達成してるか確認してStateを変更
	if (mStageClearStates.IsValidIndex(Index))
	{
		mStageClearStates[Index] = EStageClearState::Cleared;

		bool MedalFlag = IsCompleteMedal();
		bool CubeFlag = IsCompleteUsedCubes();

		if (MedalFlag && CubeFlag)
		{
			mStageClearStates[Index] = EStageClearState::FullyCleared;
		}
	}
}

void UGameProgressionSubsystem::UnLockNextStage(int32 ClearedStageIndex)
{
	if (mStageDataTable)
	{
		// 総ステージ数と解放済みステージ数が同じだったら何もせず終了
		int32 NumStages = mStageDataTable->GetRowNames().Num() - 1;
		if (NumStages == mMaxUnlockedStageIndex) return;

		// 今のステージNo.と解放済みステージ数が同じだったら次のステージをアンロック
		if (ClearedStageIndex == mMaxUnlockedStageIndex)
		{
			mMaxUnlockedStageIndex++;
		}
	}
}

// 一時保存用の辞書にしか保存されていないメダルをリセット
void UGameProgressionSubsystem::ResetMedalCollectedTemp()
{
	// 一時保存にしかないメダルの数を差分として引くことで数のつじつまを合わせる
	int32 UnconfirmedCount = 0;
	for (auto& Elem : mCollectedMedalMapTemp)
	{
		if (!mCollectedMedalMap.Contains(Elem.Key))
		{
			UnconfirmedCount++;
		}
	}
	mCurrentMedalCountArray[mCurrentStageIndex] -= UnconfirmedCount;

	// 一次保存用辞書を本体で上書きすることでリセットする
	mCollectedMedalMapTemp = mCollectedMedalMap;

	// メダルの数が変わったことを通知する
	OnMedalCountChanged.Broadcast();
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

	// リセットはInitialize以外でも呼びたいので関数化する
	ResetProgress();
}

bool UGameProgressionSubsystem::IsCompleteMedal()
{
	int32 MedalCount = GetCurrentMedalCount();
	int32 MedalMax = GetTotalMedal();

	if (MedalCount == MedalMax)
	{
		return true;
	}

	return false;
}

bool UGameProgressionSubsystem::IsCompleteUsedCubes()
{
	int32 CubeCount = GetRecordCubeCount();
	int32 CubeMax = GetTotalCube();

	if (CubeCount <= CubeMax)
	{
		return true;
	}

	return false;
}

void UGameProgressionSubsystem::SaveProgress()
{
	UCubePetsSaveGame* SaveData = Cast<UCubePetsSaveGame>(UGameplayStatics::CreateSaveGameObject(UCubePetsSaveGame::StaticClass()));

	if (SaveData)
	{
		SaveData->mMaxUnlockedStageIndex = mMaxUnlockedStageIndex;
		SaveData->mCurrentStageIndex = mCurrentStageIndex;
		SaveData->mCollectedMedalMap = mCollectedMedalMap;
		SaveData->mStageClearStates = mStageClearStates;
		SaveData->mCurrentMedalCountArray = mCurrentMedalCountArray;
		SaveData->mCurrentUsedCubeCountArray = mCurrentUsedCubeCountArray;
		SaveData->mRecordCubeCountArray = mRecordCubeCountArray;

		UGameplayStatics::SaveGameToSlot(SaveData, mSaveSlotName, 0);
	}
}

void UGameProgressionSubsystem::LoadProgress()
{
	UCubePetsSaveGame* SaveData = Cast<UCubePetsSaveGame>(UGameplayStatics::LoadGameFromSlot(mSaveSlotName, 0));

	if (!SaveData) return;

	mMaxUnlockedStageIndex = SaveData->mMaxUnlockedStageIndex;
	mCurrentStageIndex = SaveData->mCurrentStageIndex;
	mCollectedMedalMap = SaveData->mCollectedMedalMap;
	mStageClearStates = SaveData->mStageClearStates;
	mCurrentMedalCountArray = SaveData->mCurrentMedalCountArray;
	mCurrentUsedCubeCountArray = SaveData->mCurrentUsedCubeCountArray;
	mRecordCubeCountArray = SaveData->mRecordCubeCountArray;
}

bool UGameProgressionSubsystem::IsExistenceSaveData()
{
	UCubePetsSaveGame* SaveData = Cast<UCubePetsSaveGame>(UGameplayStatics::LoadGameFromSlot(mSaveSlotName, 0));

	return SaveData ? true : false;
}

#if !UE_BUILD_SHIPPING
void UGameProgressionSubsystem::UnlockAllStages()
{
	if (mStageDataTable)
	{
		// 総ステージ数を取得
		int32 NumStages = mStageDataTable->GetRowNames().Num() - 1;
		
		// 解放済みステージ数を総ステージ数と同じ数にする（＝全開放）
		mMaxUnlockedStageIndex = NumStages;
	}
}
#endif


