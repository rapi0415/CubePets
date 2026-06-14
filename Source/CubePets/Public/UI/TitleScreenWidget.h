// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "System/Title/CubePetsTitlePlayerController.h"
#include "TitleScreenWidget.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAnimationFinished);
/**
 * 
 */
UCLASS()
class CUBEPETS_API UTitleScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// BP側のアニメーションと紐づける
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> AppearanceDecoration;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> TextStartConfirmed;

	// アニメーションを再生する関数
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartAppearanceDecoration();

	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartTextStartConfirmed();
	
protected:

	UFUNCTION()
	void OnTitleAnimationFinished();

public:

	void OnIndexChanged(int32 Index);

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void BP_OnIndexChanged(int32 Index);

public:

	FOnAnimationFinished mOnAnimationFinished;

};
