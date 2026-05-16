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
			mCurrentIrisWidget->AddToViewport(1000);

			mCurrentIrisWidget->StartIrisIn();

			// リスタート用の関数をバインドしておく
			mCurrentIrisWidget->mOnFadeAnimationFinished.AddDynamic(this, &ACubePetsPlayerController::HandleAnimFinishedNotification);
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

	// アイリスアウトのアニメーションを再生する
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
	}
}

void ACubePetsPlayerController::HandleAnimFinishedNotification()
{
	// レベルのリスタート
	FName CurrentLevelName = *GetWorld()->GetName();
	UGameplayStatics::OpenLevel(this, CurrentLevelName);
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
	}
	else
	{
		SetInputMode(FInputModeGameOnly());
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
