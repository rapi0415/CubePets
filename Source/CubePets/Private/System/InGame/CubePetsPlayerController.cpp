// Fill out your copyright notice in the Description page of Project Settings.


#include "System/InGame/CubePetsPlayerController.h"
#include "Characters/CubePetsCharacter.h"
#include "UI/IrisWidget.h"
#include "UI/GameHUDWidget.h"
#include "UI/PauseWidget.h"
#include "UI/ControllGuideWidget.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerInput.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/CubePetsInputDeviceSubsystem.h"

bool ACubePetsPlayerController::InputKey(const FInputKeyParams& Params)
{
	bool bIsGamepadKey = Params.Key.IsGamepadKey();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UCubePetsInputDeviceSubsystem* Subsystem = GameInstance->GetSubsystem<UCubePetsInputDeviceSubsystem>();
		if (Subsystem)
		{
			Subsystem->NotifyDeviceChanged(bIsGamepadKey);
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

			// レベル遷移&リスタート用の関数をバインドしておく
			mCurrentIrisWidget->mOnIrisOutFinished.AddDynamic(this, &ACubePetsPlayerController::HandleLevelTransitionNotification);
		}
	}

	// インプットモード初期化処理
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	// HUDを表示
	if (mGameHUDWidgetClass)
	{
		mCurrentGameHUDWidget = CreateWidget<UGameHUDWidget>(this, mGameHUDWidgetClass);

		if (mCurrentGameHUDWidget)
		{
			mCurrentGameHUDWidget->AddToViewport();
		}
	}

	// EnhancedInput用の処理（入力マッピングの登録）
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(mSystemMappingContext, 0);
		}
	}
}

void ACubePetsPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	// 操作対象が自分のCharacterかチェックする
	if (ACubePetsCharacter* CubePetsChar = Cast<ACubePetsCharacter>(InPawn))
	{
		// Characterのデリゲートに自分の関数をバインド（リスタート用）
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
	if (mCurrentGameHUDWidget)
	{
		mCurrentGameHUDWidget->UpdateTextBlockCreate(bReticleExistence);
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
		// ポーズ中の操作用のIMCをAdd
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
			if (Subsystem)
			{
				Subsystem->AddMappingContext(mPauseMappingContext, 1);
			}
		}

		SetInputMode(FInputModeGameAndUI());
		bShowMouseCursor = true;

		// UIを表示
		if (mPauseWidgetClass && mCurrentPauseWidget == nullptr)
		{
			mCurrentPauseWidget = CreateWidget<UPauseWidget>(this, mPauseWidgetClass);
			if (mCurrentPauseWidget)
			{
				mCurrentPauseWidget->AddToViewport(100);
			}
		}

		// ステート更新
		mGameState = EGameState::PAUSE;
	}
	else
	{
		// ポーズ中の操作用のIMCをRemove
		if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
		{
			auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
			if (Subsystem)
			{
				Subsystem->RemoveMappingContext(mPauseMappingContext);
			}
		}

		SetInputMode(FInputModeGameOnly());
		bShowMouseCursor = false;

		// UIを破棄
		if (mCurrentPauseWidget)
		{
			mCurrentPauseWidget->RemoveFromParent();
			mCurrentPauseWidget = nullptr;
		}
		
		// ステート更新
		mGameState = EGameState::INGAME;
	}
}

// 上下キー
void ACubePetsPlayerController::OnPressUpDown(const FInputActionValue& Value)
{
	// ポーズ中だけ有効にする
	if (mGameState == EGameState::PAUSE)
	{
		float AxisValue = Value.Get<float>();
		int32 Direction = static_cast<int32>(AxisValue);
		ChangeIndex(Direction);
	}
}

// 決定
void ACubePetsPlayerController::OnPressDecide()
{
	if (mGameState != EGameState::PAUSE) return;

	EPauseMenuItem ChoseItem = static_cast<EPauseMenuItem>(mCurrentIndex);

	switch (ChoseItem)
	{
	case EPauseMenuItem::RESUME:

		OnResume();
		break;
	
	case EPauseMenuItem::RESTART:
	
		OnRestart();
		break;
	
	case EPauseMenuItem::RETURN_SELECT:
	
		OnReturnToSelect();
		break;

	case EPauseMenuItem::NONE:

		break;
	
	default:
		break;
	}
}

void ACubePetsPlayerController::OnResume()
{
	// ポーズ解除
	TogglePause();
}

void ACubePetsPlayerController::OnRestart()
{
	// ステージ情報をリセット
	mOnResetStageInfo.Broadcast();

	FName CurrentLevelName = *GetWorld()->GetName();
	mTargetLevelName = CurrentLevelName;

	// フェードアウトアニメーション
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
	}
}

void ACubePetsPlayerController::OnReturnToSelect()
{
	// ステージ情報をリセット
	mOnResetStageInfo.Broadcast();

	mTargetLevelName = TEXT("PL_Select");

	// フェードアウトアニメーション
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
	}
}

void ACubePetsPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mPauseAction, ETriggerEvent::Triggered, this, &ACubePetsPlayerController::TogglePause);
		EnhancedInputComponent->BindAction(mUpDownAction, ETriggerEvent::Triggered, this, &ACubePetsPlayerController::OnPressUpDown);
		EnhancedInputComponent->BindAction(mDecideAction, ETriggerEvent::Started, this, &ACubePetsPlayerController::OnPressDecide);
	}
}

void ACubePetsPlayerController::ChangeIndex(int32 Direction)
{
	int32 TargetIndex = mCurrentIndex + Direction;

	if (TargetIndex > mMaxIndex) return;
	if (TargetIndex < 0) return;

	mCurrentIndex = TargetIndex;

	if (mCurrentPauseWidget)
	{
		mCurrentPauseWidget->OnIndexChanged(mCurrentIndex);
	}

}
