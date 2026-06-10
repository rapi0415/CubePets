// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "UI/CSVTextBlock.h"
#include "Kismet/GameplayStatics.h"

void UStageSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateMedalText();
}

void UStageSelectWidget::OnIndexChanged(UTexture2D* NewTexture)
{
	BP_OnIndexChanged(NewTexture);
}

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

void UStageSelectWidget::SelectCurrentLevel()
{
	if (mLevelNameArray.IsValidIndex(mCurrentIndex))
	{
		UGameplayStatics::OpenLevel(this, mLevelNameArray[mCurrentIndex]);
	}
}

void UStageSelectWidget::UpdateMedalText()
{
	if (!TextBlockMedalCount) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Value1 = ProgressionSubsystem->GetCurrentMedalCount(mCurrentIndex);
			int32 Value2 = ProgressionSubsystem->GetTotalMedal(mCurrentIndex);

			TextBlockMedalCount->UpdateTextWithTwoInts(TEXT("Select_CollectMedal"), Value1, Value2);
		}
	}
}
