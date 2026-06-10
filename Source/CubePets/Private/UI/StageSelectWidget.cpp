// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "Kismet/GameplayStatics.h"

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
