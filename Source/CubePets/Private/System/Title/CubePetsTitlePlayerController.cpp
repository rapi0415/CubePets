// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Title/CubePetsTitlePlayerController.h"
#include "UI/TitleScreenWidget.h"
#include "UI/PreTitleWidget.h"
#include "UI/IrisWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Subsystems/CubePetsInputDeviceSubsystem.h"
#include "Subsystems/GameProgressionSubsystem.h"

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

			// （アイリスイン）タイトル画面開始アニメーション用の関数をバインドしておく
			mCurrentIrisWidget->mOnIrisInFinished.AddDynamic(this, &ACubePetsTitlePlayerController::PlayDecorationPartsAnimation);

			// （アイリスアウト）ステージ遷移用の関数をバインドしておく
			mCurrentIrisWidget->mOnIrisOutFinished.AddDynamic(this, &ACubePetsTitlePlayerController::WarpTo);
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

	// セーブデータ存在確認
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* Subsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (Subsystem)
		{
			bHasSaveData = Subsystem->IsExistenceSaveData();
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

		LoadGame();
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
		mCurrentIrisWidget->StartIrisOut();
	}

	// テキスト点滅アニメーション
	if (mCurrentTitleWidget)
	{
		mCurrentTitleWidget->StartTextStartConfirmed();
	}

	// ニューゲームなのでSubsystemの情報をリセット
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* Subsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (Subsystem)
		{
			Subsystem->ResetProgress();
		}
	}
}

void ACubePetsTitlePlayerController::LoadGame()
{
	// ロードゲームなのでSubsystemに情報をロード
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* Subsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (Subsystem)
		{
			// セーブデータがないなら何もせずに終了
			if (!Subsystem->IsExistenceSaveData()) return;

			Subsystem->LoadProgress();
		}
	}

	mCurrentTitleState = ETitleState::NONE;

	mTargetLevelName = TEXT("PL_Select");

	// フェードアウトアニメーション
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
	}

	// テキスト点滅アニメーション
	if (mCurrentTitleWidget)
	{
		mCurrentTitleWidget->StartTextLoadConfirmed();
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
	// セーブデータの有無によってIndexを変えたい
	if (bHasSaveData)
	{
		mSelectableIndices = { 0, 1, 2, 3, 4, 5 };
	}
	else
	{
		mSelectableIndices = { 0, 2, 3, 4, 5 }; // セーブデータがないときは 1番 = Load を除く
	}

	mCurrentPos = FMath::Clamp(mCurrentPos + Direction, 0, mSelectableIndices.Num() - 1);

	mCurrentIndex = mSelectableIndices[mCurrentPos];

	mCurrentTitleWidget->OnIndexChanged(mCurrentIndex);
}

bool ACubePetsTitlePlayerController::InputKey(const FInputKeyParams& Params)
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

