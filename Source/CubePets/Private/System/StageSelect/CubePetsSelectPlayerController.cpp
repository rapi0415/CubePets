// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StageSelect/CubePetsSelectPlayerController.h"
#include "UI/StageSelectWidget.h"
#include "UI/IrisWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "Subsystems/CubePetsInputDeviceSubsystem.h"
#include "Subsystems/StageClearState.h"

void ACubePetsSelectPlayerController::BeginPlay()
{
	Super::BeginPlay();

	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);

	// ステージセレクト画面を表示
	if (mStageSelectWidgetClass)
	{
		mCurrentStageSelectWidget = CreateWidget<UStageSelectWidget>(this, mStageSelectWidgetClass);

		if (mCurrentStageSelectWidget)
		{
			mCurrentStageSelectWidget->AddToViewport(0);
		}
	}

	// フェードを表示
	if (mIrisWidgetClass)
	{
		mCurrentIrisWidget = CreateWidget<UIrisWidget>(this, mIrisWidgetClass);
		if (mCurrentIrisWidget)
		{
			mCurrentIrisWidget->AddToViewport(10);

			// アイリスインを再生
			mCurrentIrisWidget->StartIrisIn();

			// 状態遷移用の関数をバインドしておく
			mCurrentIrisWidget->mOnIrisInFinished.AddDynamic(this, &ACubePetsSelectPlayerController::OnFinishIrisIn);

			// フェードアウト終了したときに呼びたい関数もバインドしておく
			mCurrentIrisWidget->mOnIrisOutFinished.AddDynamic(this, &ACubePetsSelectPlayerController::OnFinishIrisOut);
		}
	}

	// EnhancedInput用の処理（入力マッピングの登録）
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			Subsystem->AddMappingContext(mSelectMappingContext, 10);
		}
	}

	// OnIndexChangedを呼ぶためにChangeIndexを呼ぶ（遊んでたステージに帰ってくるようにする）
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Index = ProgressionSubsystem->GetCurrentStageIndex();
			ChangeIndex(Index);
		}
	}
}

void ACubePetsSelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mDecideAction, ETriggerEvent::Triggered, this, &ACubePetsSelectPlayerController::OnPressDecide);
		EnhancedInputComponent->BindAction(mLeftRightAction, ETriggerEvent::Triggered, this, &ACubePetsSelectPlayerController::OnPressLeftRight);
		EnhancedInputComponent->BindAction(mCancelAction, ETriggerEvent::Triggered, this, &ACubePetsSelectPlayerController::OnPressCancel);
	}
}

bool ACubePetsSelectPlayerController::InputKey(const FInputKeyParams& Params)
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

void ACubePetsSelectPlayerController::OnPressDecide()
{
	// アイリスインが終わるまで操作禁止
	if (!bIsActiveInput) return;

	// ステージ決定、フェードアウトして遷移する
	mNextDestination = ENextDestination::STAGE;
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
		bIsActiveInput = false;
	}
}

// タイトルに遷移するための関数
void ACubePetsSelectPlayerController::TransitionToTitle()
{
	if (!mTitleLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, mTitleLevelName);
	}
}

// ステージに遷移するための関数
void ACubePetsSelectPlayerController::TransitionToStage()
{
	if (mLevelNameArray.IsValidIndex(mCurrentIndex))
	{
		UGameplayStatics::OpenLevel(this, mLevelNameArray[mCurrentIndex]);
	}
}

void ACubePetsSelectPlayerController::OnPressLeftRight(const FInputActionValue& Value)
{
	// アイリスインが終わるまで操作禁止
	if (!bIsActiveInput) return;

	float AxisValue = Value.Get<float>();
	int32 Direction = static_cast<int32>(AxisValue);
	ChangeIndex(Direction);
}

void ACubePetsSelectPlayerController::ChangeIndex(int32 Direction)
{
	// Subsystemからステージクリア状況を取得してその範囲でIndexを更新
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 TargetIndex = mCurrentIndex + Direction;
			int32 MaxIndex = ProgressionSubsystem->GetMaxUnlockedStageIndex();

			if (TargetIndex > MaxIndex)
			{
				return;
			}

			if (TargetIndex < 0)
			{
				return;
			}

			mCurrentIndex = TargetIndex;

			// Subsystemにも記憶させておく（インゲームとかで取得したい）
			ProgressionSubsystem->SetCurrentStageIndex(mCurrentIndex);

			// Subsystemからステージクリア状況をもらう
			EStageClearState ClearState = ProgressionSubsystem->GetCurrentStageClearState(mCurrentIndex);

			// Subsystemからメダル獲得、使った箱の達成状況をもらう
			bool MedalFlag = ProgressionSubsystem->IsCompleteMedal();
			bool CubeFlag = ProgressionSubsystem->IsCompleteUsedCubes();

			// ウィジェットにIndexが変わったことを知らせる（Subsystemから値を持って来たいので条件式の中に入れている）
			if (mCurrentStageSelectWidget)
			{
				mCurrentStageSelectWidget->OnIndexChanged(mCurrentIndex, MaxIndex, ClearState, MedalFlag, CubeFlag);
			}
		}
	}
}

void ACubePetsSelectPlayerController::OnPressCancel()
{
	// アイリスインが終わるまで操作禁止
	if (!bIsActiveInput) return;

	// タイトルに戻る、フェードアウトして遷移する
	mNextDestination = ENextDestination::TITLE;
	if (mCurrentIrisWidget)
	{
		mCurrentIrisWidget->StartIrisOut();
		bIsActiveInput = false;
	}
}

void ACubePetsSelectPlayerController::OnFinishIrisIn()
{
	bIsActiveInput = true;
}

void ACubePetsSelectPlayerController::OnFinishIrisOut()
{
	switch (mNextDestination)
	{
	case ENextDestination::TITLE:
		TransitionToTitle();
		break;

	case ENextDestination::STAGE:
		TransitionToStage();
		break;

	default:
		break;
	}
	mNextDestination = ENextDestination::NONE;
}
