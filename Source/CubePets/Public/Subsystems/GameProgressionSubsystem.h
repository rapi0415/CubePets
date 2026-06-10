// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "GameProgressionSubsystem.generated.h"

USTRUCT(BlueprintType)
struct FMedalData : public FTableRowBase
{
	GENERATED_BODY()

public:

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

	// 集めたメダルの数（ステージ毎）
	UPROPERTY(BlueprintReadOnly, Category="Medal")
	TArray<int32> mCurrentMedalCountArray;

public:

	// 集めたメダルの数を取得する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	int32 GetCurrentMedalCount(int32 Index)
	{
		if (mCurrentMedalCountArray.IsValidIndex(Index))
		{
			return mCurrentMedalCountArray[Index];
		}
		return 0;
	}

	// ステージのメダル総数をデータテーブルから取得する関数
	UFUNCTION(BlueprintCallable, Category="Medal")
	int32 GetTotalMedal(int32 Index)
	{
		const FMedalData* Row = GetMedalDataByIndex(Index);
		return Row ? Row->mTotalMedal : 0;
	}

protected:

	UPROPERTY()
	TObjectPtr<UDataTable> mMedalDataTable;

	const FMedalData* GetMedalDataByIndex(int32 Index) const;

	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
};
