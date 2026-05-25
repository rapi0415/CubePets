// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePetsPlayerController.h"
#include "IrisWidget.h"
#include "Blueprint/UserWidget.h"
#include "CubePetsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ControllGuideWidget.h"
#include "GameFramework/PlayerInput.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "PauseWidget.h"

bool ACubePetsPlayerController::InputKey(const FInputKeyParams& Params)
{
	bool bIsGamepadKey = Params.Key.IsGamepadKey();

	// 前回と違うデバイスが使われたらUIを更新する
	if (bIsGamepadKey != bIsUsingGamepad)
	{
		bIsUsingGamepad = bIsGamepadKey;

		if (mCurrentControllGuideWidget)
		{
			mCurrentControllGuideWidget->UpdateDeviceIcon(bIsUsingGamepad);
		}
	}

	return Super::InputKey(Params);
}

void ACubePetsPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// アイリスインのアニメーションを再生する
	if (mIrisWidgetClass)
	{
		mCurrentIrisWidget = CreateWidget<UIrisWidget>(this, mIrisWidgetClass);

		if (mCurrentIrisWidget)
		{
			mCurrentIrisWidget->AddToViewport(100);

			mCurrentIrisWidget->StartIrisIn();

			// リスタート用の関数をバインドしておく
			mCurrentIrisWidget->mOnFadeAnimationFinished.AddDynamic(this, &ACubePetsPlayerController::HandleLevelTransitionNotification);
		}
	}

	// 初期化処理
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// 操作ガイドのUIを表示
	if (mControllGuideWidgetClass)
	{
		mCurrentControllGuideWidget = CreateWidget<UControllGuideWidget>(this, mControllGuideWidgetClass);

		if (mCurrentControllGuideWidget)
		{
			mCurrentControllGuideWidget->AddToViewport();
		}
	}

	// EnhancedInput用の処理
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(mSystemMappingContext, 10);
		}
	}

	// 開幕に画面を真っ黒にしているので解除する
	APlayerCameraManager* CameraManager = PlayerCameraManager;
	if (CameraManager)
	{
		CameraManager->StartCameraFade(1.0f, 0.0f, 0.5f, FLinearColor::Black, true, false);
	}
}

void ACubePetsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 操作対象が自分のCharacterかチェックする
	if (ACubePetsCharacter* CubePetsChar = Cast<ACubePetsCharacter>(InPawn))
	{
		// Characterのデリゲートに自分の関数をバインド
		CubePetsChar->mOnFadeOutTriggered.AddDynamic(this, &ACubePetsPlayerController::HandleFadeOutNotification);
	}

}

void ACubePetsPlayerController::HandleFadeOutNotification()
{
	FName CurrentLevelName = *GetWorld()->GetName();
	RequestLevelTransition(CurrentLevelName);
}

void ACubePetsPlayerController::HandleLevelTransitionNotification()
{
	// レベルの遷移
	if(!mTargetLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, mTargetLevelName);
	}
}

void ACubePetsPlayerController::RequestLevelTransition(FName TargetLevelName)
{
	// レベル名を設定
	mTargetLevelName = TargetLevelName;

	// フェードアウトを再生
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
	}
}

// レティクルの有無が変わった時にプレイヤーから呼んでもらう関数、ウィジェットのテキストブロックを更新
void ACubePetsPlayerController::NotifyReticleStateChanged(bool bReticleExistence)
{
	if (mCurrentControllGuideWidget)
	{
		mCurrentControllGuideWidget->UpdateTextBlockCreate(bReticleExistence);
	}
}

// ポーズ切り替え
void ACubePetsPlayerController::TogglePause()
{
	UWorld* World = GetWorld();
	if (!World) return;

	bool bNewPauseState = !UGameplayStatics::IsGamePaused(World);
	UGameplayStatics::SetGamePaused(World, bNewPauseState);

	if (bNewPauseState)
	{
		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;

		// UIを表示
		if (mPauseWidgetClass && mCurrentPauseWidget == nullptr)
		{
			mCurrentPauseWidget = CreateWidget<UPauseWidget>(this, mPauseWidgetClass);
			if (mCurrentPauseWidget)
			{
				mCurrentPauseWidget->AddToViewport(1000);
			}
		}
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		// UIを破棄
		if (mCurrentPauseWidget)
		{
			mCurrentPauseWidget->RemoveFromParent();
			mCurrentPauseWidget = nullptr;
		}
	}
}

void ACubePetsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mPauseAction, ETriggerEvent::Triggered, this, &ACubePetsPlayerController::TogglePause);
	}
}
