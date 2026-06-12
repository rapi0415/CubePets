// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "GameProgressionSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMedalCountChanged);

USTRUCT(BlueprintType)
struct FMedalData : public FTableRowBase
{
	GENERATED_BODY()

public:

	// メダルの総数（ステージ毎）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Medal")
	int32 mTotalMedal = 0;
};
/**
 * 
 */
UCLASS()
class CUBEPETS_API UGameProgressionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:

	FOnMedalCountChanged OnMedalCountChanged;

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

	// 集めたメダルの数を取得する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	int32 GetCurrentMedalCount()
	{
		if (mCurrentMedalCountArray.IsValidIndex(mCurrentStageIndex))
		{
			return mCurrentMedalCountArray[mCurrentStageIndex];
		}
		return 0;
	}

	// 集めたメダルの数を更新する関数
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

	// ステージのメダル総数をデータテーブルから取得する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	int32 GetTotalMedal()
	{
		const FMedalData* Row = GetMedalDataByIndex(mCurrentStageIndex);
		return Row ? Row->mTotalMedal : 0;
	}

protected:

	UPROPERTY()
	TObjectPtr<UDataTable> mMedalDataTable;

	const FMedalData* GetMedalDataByIndex(int32 Index) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
