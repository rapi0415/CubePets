// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "UI/CSVTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UI/StageStatusWidget.h"

void UStageSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	/*
	// Subsystemから今のステージ番号を取得して更新（ステージから戻ってきたときに遊んでたステージから開始させるため）
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 StageIndex = ProgressionSubsystem->GetCurrentStageIndex();
			ChangeIndex(StageIndex); // 最初は0なのでステージ番号を足せばいい
		}
	}
	*/

	UpdateMedalText();
	UpdateCubeText();
}

/*
void UStageSelectWidget::StartDecideRightArrow()
{
	if (DecideRightArrow)
	{
		PlayAnimation(DecideRightArrow);
	}
}
*/

void UStageSelectWidget::OnIndexChanged(int32 Index, int32 MaxIndex, EStageClearState ClearState)
{
	BP_OnIndexChanged(Index, MaxIndex, ClearState);
}

/*
void UStageSelectWidget::ChangeIndex(int32 Direction)
{
	if (mImageArray.Num() == 0) return;

	// Subsystemからステージクリア状況を取得してその範囲でIndexを更新
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 TargetIndex = mCurrentIndex + Direction;

			if (TargetIndex > ProgressionSubsystem->GetMaxUnlockedStageIndex())
			{
				return;
			}

			if (TargetIndex < 0)
			{
				return;
			}

			mCurrentIndex = TargetIndex;

			// Subsystemにも記憶させておく（インゲームとかで取得したい）
			ProgressionSubsystem->SetCurrentStageIndex(mCurrentIndex);
		}
	}

	// 画像を取得する
	if (mImageArray[mCurrentIndex].IsValid())
	{
		OnIndexChanged(mImageArray[mCurrentIndex].Get());
	}
	else
	{
		OnIndexChanged(mImageArray[mCurrentIndex].LoadSynchronous());
	}
}
*/

/*
void UStageSelectWidget::SelectCurrentLevel()
{
	if (mLevelNameArray.IsValidIndex(mCurrentIndex))
	{
		UGameplayStatics::OpenLevel(this, mLevelNameArray[mCurrentIndex]);
	}
}
*/

void UStageSelectWidget::UpdateMedalText()
{
	if (!TextBlockMedalCount) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Value1 = ProgressionSubsystem->GetCurrentMedalCount();
			int32 Value2 = ProgressionSubsystem->GetTotalMedal();

			TextBlockMedalCount->UpdateTextWithTwoInts(TEXT("Select_CollectMedal"), Value1, Value2);
		}
	}
}

void UStageSelectWidget::UpdateCubeText()
{
	if (!TextBlockCubeCount) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Value1 = ProgressionSubsystem->GetRecordCubeCount();
			int32 Value2 = ProgressionSubsystem->GetTotalCube();

			TextBlockCubeCount->UpdateTextWithTwoInts(TEXT("Select_UsedCubes"), Value1, Value2);
		}
	}
}

