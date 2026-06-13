// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Title/CubePetsTitlePlayerController.h"
#include "UI/TitleScreenWidget.h"
#include "UI/PreTitleWidget.h"
#include "UI/IrisWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

void ACubePetsTitlePlayerController::BeginPlay()
{
	Super::BeginPlay();


	// インプットモード初期化処理
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	// タイトル画面を表示
	if (mTitleWidgetClass)
	{
		mCurrentTitleWidget = CreateWidget<UTitleScreenWidget>(this, mTitleWidgetClass);

		if (mCurrentTitleWidget)
		{
			mCurrentTitleWidget->AddToViewport(0);
		}
	}

	// フェードを表示
	if (mIrisWidgetClass)
	{
		mCurrentIrisWidget = CreateWidget<UIrisWidget>(this, mIrisWidgetClass);
		if (mCurrentIrisWidget)
		{
			mCurrentIrisWidget->AddToViewport(10);
		}
	}

	// プレタイトルを表示
	if (mPreTitleWidgetClass)
	{
		mCurrentPreTitleWidget = CreateWidget<UPreTitleWidget>(this, mPreTitleWidgetClass);
		if (mCurrentPreTitleWidget)
		{
			mCurrentPreTitleWidget->AddToViewport(100);
		}
	}

	// EnhancedInput用の処理（入力マッピングの登録）
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(mPreTitleMappingContext, 10);
		}
	}
}

void ACubePetsTitlePlayerController::OnPressConfirm()
{
	if (bIsStarted) return;

	bIsStarted = true;

	// プレタイトル用のIMCを外す
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->RemoveMappingContext(mPreTitleMappingContext);
		}
	}

	// プレタイトルを消す
	if (mCurrentPreTitleWidget)
	{
		mCurrentPreTitleWidget->RemoveFromParent();
		mCurrentPreTitleWidget = nullptr;
	}

	// アイリスインのアニメーションを再生する
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisIn();

		// タイトル画面開始アニメーション用の関数をバインドしておく
		mCurrentIrisWidget->mOnIrisInFinished.AddDynamic(this, &ACubePetsTitlePlayerController::PlayDecorationPartsAnimation);
	}
}

void ACubePetsTitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mConfirmAction, ETriggerEvent::Triggered, this, &ACubePetsTitlePlayerController::OnPressConfirm);
	}
}

bool ACubePetsTitlePlayerController::InputKey(const FInputKeyParams& Params)
{
	bool bIsGamepadKey = Params.Key.IsGamepadKey();

	// 前回と違うデバイスが使われたらUIを更新する
	if (bIsGamepadKey != bIsUsingGamepad)
	{
		bIsUsingGamepad = bIsGamepadKey;

		if (mCurrentPreTitleWidget)
		{
			mCurrentPreTitleWidget->UpdateDeviceIcon(bIsUsingGamepad);
		}
	}

	return Super::InputKey(Params);
}

void ACubePetsTitlePlayerController::PlayDecorationPartsAnimation()
{
	// タイトルウィジェットのアニメーション再生用関数を呼び出す
	if (mCurrentTitleWidget)
	{
		mCurrentTitleWidget->StartAppearanceDecoration();
	}
}

