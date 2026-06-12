// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsPlayerController.generated.h"

class UIrisWidget;
class UGameHUDWidget;
class UInputAction;
class UInputMappingContext;
class UPauseWidget;

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

	// インプット系
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> mSystemMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mPauseAction = nullptr;

	UFUNCTION(BlueprintCallable, Category="Pause")
	void TogglePause();

	virtual void SetupInputComponent() override;

};
