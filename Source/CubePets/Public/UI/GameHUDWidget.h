// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUDWidget.generated.h"

class ControllGuideWidget;
class StageStatusWidget;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UGameHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget), Category="UI")
	TObjectPtr<UUserWidget> ControlGuide = nullptr;

	UPROPERTY(BlueprintreadOnly, meta=(BindWidget), Category="UI")
	TObjectPtr<UUserWidget> StageStatus = nullptr;

public:

	// ControlGuideÇÃä÷êîÇåƒÇ—èoÇ∑ÇΩÇﬂÇÃä÷êî
	/*
	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateDeviceIcon(bool bIsGamepad);
	*/

	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateTextBlockCreate(bool bIsReticleExistence);
};
