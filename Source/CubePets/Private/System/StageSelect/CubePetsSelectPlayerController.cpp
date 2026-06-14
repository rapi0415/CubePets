// Fill out your copyright notice in the Description page of Project Settings.


#include "System/StageSelect/CubePetsSelectPlayerController.h"
#include "UI/StageSelectWidget.h"
#include "UI/IrisWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameProgressionSubsystem.h"

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

	// 初回も0を渡して関数を呼ぶ（OnIndexChangedを呼ぶため）
	ChangeIndex(0);

}

void ACubePetsSelectPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mConfirmAction, ETriggerEvent::Triggered, this, &ACubePetsSelectPlayerController::OnPressConfirm);
		EnhancedInputComponent->BindAction(mLeftRightAction, ETriggerEvent::Triggered, this, &ACubePetsSelectPlayerController::OnPressLeftRight);
	}
}

void ACubePetsSelectPlayerController::OnPressConfirm()
{
	// アイリスインが終わるまで操作禁止
	if (!bIsIrisInFinished) return;

	OpenCurrentLevel();
}

void ACubePetsSelectPlayerController::OpenCurrentLevel()
{
	if (mLevelNameArray.IsValidIndex(mCurrentIndex))
	{
		UGameplayStatics::OpenLevel(this, mLevelNameArray[mCurrentIndex]);
	}
}

void ACubePetsSelectPlayerController::OnPressLeftRight(const FInputActionValue& Value)
{
	// アイリスインが終わるまで操作禁止
	if (!bIsIrisInFinished) return;

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

			if (TargetIndex > ProgressionSubsystem->GetMaxUnlockedStageIndex())
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
		}
	}

	if (mCurrentStageSelectWidget)
	{
		mCurrentStageSelectWidget->OnIndexChanged(mCurrentIndex);
	}
}

void ACubePetsSelectPlayerController::OnFinishIrisIn()
{
	bIsIrisInFinished = true;
}
