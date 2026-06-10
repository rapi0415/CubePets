// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectWidget.h"
#include "Kismet/GameplayStatics.h"

void UStageSelectWidget::OnIndexChanged(UTexture2D* NewTexture)
{
	BP_OnIndexChanged(NewTexture);
}

void UStageSelectWidget::ChangeIndex(int32 Direction)
{
	if (mImageArray.Num() == 0) return;

	// Indexを更新、ループするようにする
	mCurrentIndex += Direction;

	if (mCurrentIndex >= mImageArray.Num())
	{
		mCurrentIndex = 0;
	}
	else if (mCurrentIndex < 0)
	{
		mCurrentIndex = mImageArray.Num() - 1;
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
