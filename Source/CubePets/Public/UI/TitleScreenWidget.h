// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleScreenWidget.generated.h"

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

	// アニメーションを再生する関数
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartAppearanceDecoration();

public:

	// 自分にMappingContextをバインドする関数
	void SetIMC();

	UFUNCTION(BlueprintImplementableEvent, Category = "Input")
	void BP_SetIMC();
	
protected:

	UFUNCTION()
	void OnTitleAnimationFinished();

protected:

	void OnIndexChanged();

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void BP_OnIndexChanged();

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeIndex(int32 Direction);

protected:

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int32 mCurrentIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	int32 mMaxIndex = 5;


};
