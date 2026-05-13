// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePetsTitlePlayerController.h"
#include "TitleWidget.h"
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

			/*
			// スタートボタンを押したときに呼びたいイベントをバインド
			mCurrentTitleWidget->mOnStartButtonPressed.AddDynamic(this, &ACubePetsTitlePlayerController::HandleGameStartNotification);

			// mCurrentTitleWidget->SetupInitialFocus();
			*/
		}
	}

	// マウスカーソルを表示
	bShowMouseCursor = true;
}

// ゲームスタート
void ACubePetsTitlePlayerController::HandleGameStartNotification()
{
	if (!mStartLevel.IsNull())
	{
		UGameplayStatics::OpenLevelBySoftObjectPtr(this, mStartLevel);
		UE_LOG(LogTemp, Warning, TEXT("GameStart"));
	}
}
