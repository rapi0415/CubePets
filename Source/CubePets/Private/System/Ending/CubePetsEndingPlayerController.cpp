// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Ending/CubePetsEndingPlayerController.h"
#include "UI/EndingWidget.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/CubePetsInputDeviceSubsystem.h"
#include "Subsystems/GameProgressionSubsystem.h"

void ACubePetsEndingPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// ウィジェットを生成
	if (mEndingWidgetClass)
	{
		mCurrentEndingWidget = CreateWidget<UEndingWidget>(this, mEndingWidgetClass);
		if (mCurrentEndingWidget)
		{
			mCurrentEndingWidget->AddToViewport();

			mCurrentEndingWidget->mOnAppearanceAnimationFinished.AddDynamic(this, &ACubePetsEndingPlayerController::ActiveInput);
		}
	}

	// EnhancedInput用の処理（入力マッピングの登録）
	if (ULocalPlayer* LocalPlayer = GetLocalPlayer())
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
		{
			if(mEndingMappingContext)
			{ 
				Subsystem->AddMappingContext(mEndingMappingContext, 10);
			}
		}
	}

	// セーブする
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			ProgressionSubsystem->SaveProgress();
		}
	}
}

void ACubePetsEndingPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(mDecideAction, ETriggerEvent::Triggered, this, &ACubePetsEndingPlayerController::OnPressDecide);
	}
}

void ACubePetsEndingPlayerController::OnPressDecide()
{
	// 操作不能のときは何もしない
	if (!bIsActive) return;

	// タイトルに戻る
	if (!mTitleLevelName.IsNone())
	{
		UGameplayStatics::OpenLevel(this, mTitleLevelName);
		bIsActive = false;
	}
}

void ACubePetsEndingPlayerController::ActiveInput()
{
	bIsActive = true;
}

bool ACubePetsEndingPlayerController::InputKey(const FInputKeyParams& Params)
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
