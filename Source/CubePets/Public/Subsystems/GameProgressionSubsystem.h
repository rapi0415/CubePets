// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "Enums/StageClearState.h"
#include "GameProgressionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMedalCountChanged);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUsedCubeCountChanged);

USTRUCT(BlueprintType)
struct FStageData : public FTableRowBase
{
	GENERATED_BODY()

public:

	// メダルの総数（ステージ毎）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Medal")
	int32 mTotalMedal = 0;

	// 箱の使用目標数（ステージ毎）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Cube")
	int32 mTotalCube = 0;
};

/**
 * 
 */
UCLASS()
class CUBEPETS_API UGameProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

	friend class UDebugMenuSubsystem;
	
public:

	FOnMedalCountChanged OnMedalCountChanged;
	FOnUsedCubeCountChanged OnUsedCubeCountChanged;

protected:

	// ステージのクリア状況管理用配列
	TArray<EStageClearState> mStageClearStates;

public:

	// Subsystemが持っている情報をリセットする関数
	void ResetProgress();

public:

	// 指定されたステージのクリア状況を返す関数
	UFUNCTION()
	EStageClearState GetCurrentStageClearState(int32 Index)
	{
		if (mStageClearStates.IsValidIndex(Index))
		{
			return mStageClearStates[Index];
		}
		return EStageClearState::NotCleared;
	}

	UFUNCTION()
	void UpdateCurrentStageClearState(int32 Index);

protected:

	// 解放されているステージの総数
	UPROPERTY(BlueprintReadOnly, Category="Stage")
	int32 mMaxUnlockedStageIndex = 0;

public:

	// get（ステージの総数）
	UFUNCTION(BlueprintCallable, Category="Stage")
	int32 GetMaxUnlockedStageIndex()
	{
		return mMaxUnlockedStageIndex;
	}

	// 次のステージを解放する
	UFUNCTION(BlueprintCallable, Category="Stage")
	void UnLockNextStage(int32 ClearedStageIndex);

protected:

	// Subsystemにもステージ番号を記憶させておく（インゲームとかで取得したい）
	UPROPERTY(BlueprintReadOnly, Category="UI")
	int32 mCurrentStageIndex = 0;

public:

	// get（ステージ番号）
	UFUNCTION(BlueprintCallable, Category="UI")
	int32 GetCurrentStageIndex()
	{
		return mCurrentStageIndex;
	}

	// set（ステージ番号）
	UFUNCTION(BlueprintCallable, Category="UI")
	void SetCurrentStageIndex(int32 Index)
	{
		mCurrentStageIndex = Index;
	}

protected:

	// 集めたメダルの数（ステージ毎）
	UPROPERTY(BlueprintReadOnly, Category="Medal")
	TArray<int32> mCurrentMedalCountArray;

public:

	// get 集めたメダルの数を取得する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	int32 GetCurrentMedalCount()
	{
		if (mCurrentMedalCountArray.IsValidIndex(mCurrentStageIndex))
		{
			return mCurrentMedalCountArray[mCurrentStageIndex];
		}
		return 0;
	}

	// add 集めたメダルの数を更新する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	void AddMedalCount(int32 StageIndex)
	{
		if (mCurrentMedalCountArray.IsValidIndex(StageIndex))
		{
			// 集めたメダル+1
			mCurrentMedalCountArray[StageIndex]++;

			// メダルが増えたことを通知する
			OnMedalCountChanged.Broadcast();
		}
	}

	UFUNCTION(BlueprintCallable, Category = "Medal")
	void SubtractMedalCount(int32 StageIndex)
	{
		if (mCurrentMedalCountArray.IsValidIndex(StageIndex))
		{
			// 集めたメダル-1
			mCurrentMedalCountArray[StageIndex]--;
		}
	}

public:

	// ステージのメダル総数をデータテーブルから取得する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	int32 GetTotalMedal()
	{
		const FStageData* Row = GetStageDataByIndex(mCurrentStageIndex);
		return Row ? Row->mTotalMedal : 0;
	}

public:

	// 獲得済みのメダルのIDを保存する辞書
	UPROPERTY(BlueprintReadOnly, Category = "Medal")
	TMap<FName, bool> mCollectedMedalMap;

	// メダルIDを保存する辞書（一時保存用）※チェックポイントに触れるまでは未確定にしたい
	UPROPERTY()
	TMap<FName, bool> mCollectedMedalMapTemp;

	// メダルがすでに獲得済みかチェックする関数
	UFUNCTION(BlueprintCallable, Category = "Medal")
	bool IsMedalAlreadyCollected(FName MedalID) const
	{
		if (mCollectedMedalMap.Contains(MedalID))
		{
			return mCollectedMedalMap[MedalID];
		}
		return false;
	}

	// メダルを"一時的"に獲得済みにする関数
	UFUNCTION()
	void SetMedalCollectedTemp(FName MedalId)
	{
		if (!MedalId.IsNone())
		{
			mCollectedMedalMapTemp.Add(MedalId, true);
		}
	}

	// メダルを獲得済み（確定）にする関数
	UFUNCTION(BlueprintCallable, Category = "Medal")
	void SetMedalCollected()
	{
		/*
		if (!MedalID.IsNone())
		{
			mCollectedMedalMap.Add(MedalID, true);
		}
		*/
		mCollectedMedalMap = mCollectedMedalMapTemp;
	}

	// メダルの獲得状況をリセットする関数
	UFUNCTION(BlueprintCallable, Category = "Medal")
	void ResetMedalCollected(FName MedalID)
	{
		mCollectedMedalMap.Remove(MedalID);
	}

	UFUNCTION()
	void ResetMedalCollectedTemp();

	// やり直し時にそのステージのメダル獲得状況をリセットする関数
	/*
	UFUNCTION()
	void ResetStageMedalInfo();
	*/

protected:

	// 使った箱の数（ステージ毎）
	UPROPERTY(BlueprintReadOnly, Category = "Cube")
	TArray<int32> mCurrentUsedCubeCountArray;

	// 使った箱の数（各ステージの最高記録保持用）
	UPROPERTY(BlueprintReadOnly, Category = "Cube")
	TArray<int32> mRecordCubeCountArray;

	// 使った箱の数（チェックポイント保存用）
	UPROPERTY(BlueprintReadOnly, Category = "Cube")
	int32 mCheckPointCubeCount = 0;

public:

	// get 使った箱の数を取得する関数
	UFUNCTION(BlueprintCallable, Category = "Cube")
	int32 GetCurrentUsedCubeCount()
	{
		if (mCurrentUsedCubeCountArray.IsValidIndex(mCurrentStageIndex))
		{
			return mCurrentUsedCubeCountArray[mCurrentStageIndex];
		}
		return 0;
	}

	// add 使った箱の数を+1する関数
	UFUNCTION(BlueprintCallable, Category = "Cube")
	void AddUsedCubeCount()
	{
		if (mCurrentUsedCubeCountArray.IsValidIndex(mCurrentStageIndex))
		{
			// 使った箱+1（99で止めておく）
			int32& Count = mCurrentUsedCubeCountArray[mCurrentStageIndex];
			Count = FMath::Min(Count + 1, 99);

			// 使った箱の数が変わったことを通知する
			OnUsedCubeCountChanged.Broadcast();
		}
	}

	// 使った箱の数をリセットする関数
	UFUNCTION(BlueprintCallable, Category="Cube")
	void ResetUsedCubeCount()
	{
		if (mCurrentUsedCubeCountArray.IsValidIndex(mCurrentStageIndex))
		{
			mCurrentUsedCubeCountArray[mCurrentStageIndex] = 0;

			// 使った箱の数が変わったことを通知する
			// OnUsedCubeCountChanged.Broadcast();
		}
	}

	// 使った箱の最高記録を取得する関数
	UFUNCTION(BlueprintCallable, Category = "CUbe")
	int32 GetRecordCubeCount()
	{
		if (mRecordCubeCountArray.IsValidIndex(mCurrentStageIndex))
		{
			return mRecordCubeCountArray[mCurrentStageIndex];
		}
		return 0;
	}

	// 使った箱の最高記録を更新する関数
	UFUNCTION(BlueprintCallable, Category = "Cube")
	void UpdateRecordCubeCount()
	{
		int32& Record = mRecordCubeCountArray[mCurrentStageIndex];
		Record = FMath::Min(Record, mCurrentUsedCubeCountArray[mCurrentStageIndex]);
	}

	// 使った箱の数（チェックポイント保存用）を保存しておく関数
	UFUNCTION()
	void UpdateCheckPointCubeCount()
	{
		mCheckPointCubeCount = GetCurrentUsedCubeCount();
	}

	// 使った箱の数をチェックポイント通過時の数に戻す関数
	UFUNCTION()
	void RestoreToSavedCubeCount()
	{
		if (mCurrentUsedCubeCountArray.IsValidIndex(mCurrentStageIndex))
		{
			mCurrentUsedCubeCountArray[mCurrentStageIndex] = mCheckPointCubeCount;

			// 使った箱の数が変わったことを通知する
			OnUsedCubeCountChanged.Broadcast();
		}
	}

	// 使った箱の数（チェックポイント保存用）をリセットする関数
	UFUNCTION()
	void ResetCheckPointCubeCount()
	{
		mCheckPointCubeCount = 0;
	}

public:

	// ステージの箱使用目標数をデータテーブルから取得する関数
	UFUNCTION(BlueprintCallable, Category="Cube")
	int32 GetTotalCube()
	{
		const FStageData* Row = GetStageDataByIndex(mCurrentStageIndex);
		return Row ? Row->mTotalCube : 0;
	}

protected:

	// データテーブル
	UPROPERTY()
	TObjectPtr<UDataTable> mStageDataTable;

	const FStageData* GetStageDataByIndex(int32 Index) const;

protected:

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

public:
	
	// メダル獲得を達成しているかどうか返す関数
	UFUNCTION()
	bool IsCompleteMedal();

	// 使った箱を達成しているかどうか返す関数
	UFUNCTION()
	bool IsCompleteUsedCubes();

public:
	
	// セーブ
	UFUNCTION()
	void SaveProgress();

	// ロード
	UFUNCTION()
	void LoadProgress();

	// セーブデータがあるか確かめる関数
	UFUNCTION(BlueprintCallable, Category = "SaveData")
	bool IsExistenceSaveData();

	static const FString mSaveSlotName;

protected:

#if !UE_BUILD_SHIPPING

	// 全ステージ解放する関数（デバッグ用）
	void UnlockAllStages();

#endif


};
