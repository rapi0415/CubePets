// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StageSelect/CubePetsSelectPlayerController.h"
#include "UI/StageSelectWidget.h"

void ACubePetsSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ステージセレクト画面を表示
	if (mStageSelectWidgetClass)
	{
		mCurrentStageSelectWidget = CreateWidget<UStageSelectWidget>(this, mStageSelectWidgetClass);

		if (mCurrentStageSelectWidget)
		{
			mCurrentStageSelectWidget->AddToViewport(100);

			FInputModeGameAndUI InputMode;
			SetInputMode(InputMode);

			mCurrentStageSelectWidget->SetUserFocus(this);
		}
	}
}
