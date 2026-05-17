// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsPlayerController.generated.h"

class UIrisWidget;
class UControllGuideWidget;
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

	// 操作ガイド用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UControllGuideWidget> mControllGuideWidgetClass = nullptr;

	// ポーズ用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UPauseWidget> mPauseWidgetClass = nullptr;

protected:

	virtual bool InputKey(const FInputKeyParams& Params) override;

	bool bIsUsingGamepad = false;

protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;

	UFUNCTION()
	void HandleFadeOutNotification();
	
	UFUNCTION()
	void HandleAnimFinishedNotification();

public:

	void NotifyReticleStateChanged(bool bReticleExistence);

private:

	// フェードウィジェット
	UPROPERTY()
	TObjectPtr<UIrisWidget> mCurrentIrisWidget = nullptr;

	// 操作ガイドウィジェット
	UPROPERTY()
	TObjectPtr<UControllGuideWidget> mCurrentControllGuideWidget = nullptr;

	// ポーズウィジェット
	UPROPERTY()
	TObjectPtr<UPauseWidget> mCurrentPauseWidget = nullptr;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> mSystemMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mPauseAction = nullptr;

	UFUNCTION(BlueprintCallable, Category="Pause")
	void TogglePause();

	virtual void SetupInputComponent() override;

};
