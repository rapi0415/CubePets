// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndingWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAppearanceAnimationFinished);

/**
 * 
 */
UCLASS()
class CUBEPETS_API UEndingWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;

protected:

	// BP側のアニメーションと紐づける
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> AppearanceText;

public:

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnAppearanceAnimationFinished mOnAppearanceAnimationFinished;

public:

	UFUNCTION()
	void StartAppearanceText();

protected:

	UFUNCTION()
	void OnAppearanceAnimationFinished();

protected:

	UFUNCTION()
	void ShowButton();

	UFUNCTION(BlueprintImplementableEvent, Category = "Button")
	void BP_ShowButton();


	
};
