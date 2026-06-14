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
			Subsystem->AddMappingContext(mTitleMappingContext, 10);
		}
	}
}

// 決定キー
void ACubePetsTitlePlayerController::OnPressConfirm()
{
	switch (mCurrentTitleState)
	{
	case ETitleState::PRE_TITLE:
		
		HandlePreTitleConfirm();
		break;

	case ETitleState::MAIN_TITLE:
		
		HandleMainTitleConfirm();
		break;

	case ETitleState::NONE:
		
		// 何もしない 
		break;

	default:
		break;
	}
}

// 開始画面で決定ボタンを押したときの処理
void ACubePetsTitlePlayerController::HandlePreTitleConfirm()
{
	mCurrentTitleState = ETitleState::NONE;

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

// タイトル画面（メイン）での決定ボタン処理
void ACubePetsTitlePlayerController::HandleMainTitleConfirm()
{
	ETitleMenuItem ChoseItem = static_cast<ETitleMenuItem>(mCurrentIndex);
	OnMenuConfirmed(ChoseItem);
}

void ACubePetsTitlePlayerController::OnMenuConfirmed(ETitleMenuItem ChosenItem)
{
	switch (ChosenItem)
	{
	case ETitleMenuItem::START:
		
		StartGame();
		break;

	case ETitleMenuItem::LOAD:
		break;
	case ETitleMenuItem::JAPANESE:
		break;
	case ETitleMenuItem::ENGLISH:
		break;
	case ETitleMenuItem::SHOP:
		break;
	case ETitleMenuItem::QUIT:
		break;
	default:
		break;
	}
}

void ACubePetsTitlePlayerController::StartGame()
{
	mCurrentTitleState = ETitleState::NONE;

	mTargetLevelName = TEXT("PL_Select");

	// フェードアウトアニメーション
	if (mCurrentIrisWidget)
	{
		// ステージ遷移用の関数をバインドしておく
		mCurrentIrisWidget->mOnIrisOutFinished.AddDynamic(this, &ACubePetsTitlePlayerController::WarpTo);

		mCurrentIrisWidget->StartIrisOut();
	}

	// テキスト点滅アニメーション
	if (mCurrentTitleWidget)
	{
		mCurrentTitleWidget->StartTextStartConfirmed();
	}
}

void ACubePetsTitlePlayerController::WarpTo()
{
	// レベルの遷移
	if (!mTargetLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, mTargetLevelName);
	}
}


// 上下キー
void ACubePetsTitlePlayerController::OnPressUpDown(const FInputActionValue& Value)
{
	// 操作不能状態のときはすぐにreturn
	if (mCurrentTitleState == ETitleState::NONE) return;

	if (mCurrentTitleWidget)
	{
		float AxisValue = Value.Get<float>();
		int32 Direction = static_cast<int32>(AxisValue);
		ChangeIndex(Direction);
	}
}

void ACubePetsTitlePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mConfirmAction, ETriggerEvent::Triggered, this, &ACubePetsTitlePlayerController::OnPressConfirm);
		EnhancedInputComponent->BindAction(mUpDownAction, ETriggerEvent::Triggered, this, &ACubePetsTitlePlayerController::OnPressUpDown);
	}
}

// タイトル画面操作用の状態に変更する関数（アニメーションの終了にバインドしたかったので専用の関数として作っている）
void ACubePetsTitlePlayerController::ChangeTitleStateToMainTitle()
{
	mCurrentTitleState = ETitleState::MAIN_TITLE;
}

void ACubePetsTitlePlayerController::ChangeIndex(int32 Direction)
{
	int32 TargetIndex = mCurrentIndex + Direction;

	if (TargetIndex > mMaxIndex)
	{
		return;
	}

	if (TargetIndex < 0)
	{
		return;
	}

	mCurrentIndex = TargetIndex;

	mCurrentTitleWidget->OnIndexChanged(mCurrentIndex);
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
		// 状態遷移用の関数をバインドしておく
		mCurrentTitleWidget->mOnAnimationFinished.AddDynamic(this, &ACubePetsTitlePlayerController::ChangeTitleStateToMainTitle);

		mCurrentTitleWidget->StartAppearanceDecoration();
	}
}

