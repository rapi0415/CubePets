// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsPlayerController.generated.h"

class UIrisWidget;
class UControllGuideWidget;

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

	// ウィジェット
	UPROPERTY()
	UIrisWidget* mCurrentIrisWidget = nullptr;

	UPROPERTY()
	UControllGuideWidget* mCurrentControllGuideWidget = nullptr;

};
