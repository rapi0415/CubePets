// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CubePetsPlayerController.generated.h"

class UIrisWidget;
class UGameHUDWidget;
class UInputAction;
class UInputMappingContext;
class UPauseWidget;

UENUM(BlueprintType)
enum class EGameState : uint8
{
	NONE,
	INGAME,
	PAUSE,
};

UENUM(BlueprintType)
enum class EPauseMenuItem : uint8
{
	RESUME,
	RESTART,
	RETURN_SELECT,
};

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	
	// フェード演出用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UIrisWidget> mIrisWidgetClass = nullptr;

	// HUD用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UGameHUDWidget> mGameHUDWidgetClass = nullptr;

	// ポーズ用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UPauseWidget> mPauseWidgetClass = nullptr;

protected:

	// 入力デバイスの検知
	virtual bool InputKey(const FInputKeyParams& Params) override;

	bool bIsUsingGamepad = false;

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

protected:

	// フェードアウトを再生する関数
	UFUNCTION()
	void HandleFadeOutNotification();
	
	// レベルの遷移をする関数
	UFUNCTION()
	void HandleLevelTransitionNotification();

protected:

	FName mTargetLevelName;

public:

	// 
	UFUNCTION()
	void RequestLevelTransition(FName TargetLevelName);

public:

	void NotifyReticleStateChanged(bool bReticleExistence);

private:

	// フェードウィジェット
	UPROPERTY(Transient)
	TObjectPtr<UIrisWidget> mCurrentIrisWidget = nullptr;

	// HUDウィジェット
	UPROPERTY(Transient)
	TObjectPtr<UGameHUDWidget> mCurrentGameHUDWidget = nullptr;

	// ポーズウィジェット
	UPROPERTY(Transient)
	TObjectPtr<UPauseWidget> mCurrentPauseWidget = nullptr;

protected:

	EGameState mGameState = EGameState::NONE;
	EPauseMenuItem mPauseMenuItem = EPauseMenuItem::RESUME;

	int32 mCurrentIndex = 0;

	UPROPERTY(EditAnywhere, Category = "UI")
	int32 mMaxIndex = 2;

protected:

	// インプット系
	// IMC（ポーズ用）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> mSystemMappingContext = nullptr;

	// IMC（ポーズメニュー）
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> mPauseMappingContext = nullptr;

	// ポーズ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mPauseAction = nullptr;

	// 上下キー
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mUpDownAction = nullptr;

	// 決定
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mDecideAction = nullptr;

protected:

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void TogglePause();

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void OnPressUpDown(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void OnPressDecide();

	void OnResume();
	void OnReturnSelect();

	virtual void SetupInputComponent() override;

public:

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void ChangeIndex(int32 Direction);

};
