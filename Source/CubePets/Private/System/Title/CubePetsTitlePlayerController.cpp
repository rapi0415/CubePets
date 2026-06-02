// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Title/CubePetsTitlePlayerController.h"
#include "UI/TitleWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"

void ACubePetsTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();

	// タイトル画面を表示
	if (mTitleWidgetClass)
	{
		mCurrentTitleWidget = CreateWidget<UTitleWidget>(this, mTitleWidgetClass);

		if (mCurrentTitleWidget)
		{
			mCurrentTitleWidget->AddToViewport();

			// UI専用の入力モードにする
			FInputModeUIOnly InputMode;
			SetInputMode(InputMode);
		}
	}

	// マウスカーソルを表示
	bShowMouseCursor = true;
}

