// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IrisWidget.generated.h"

class UImage;
class UWidgetAnimation;

// アニメーション終了通知用のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeAnimationFinished);

UCLASS()
class CUBEPETS_API UIrisWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	// BP側のパーツと紐づける
	UPROPERTY(Transient, meta = (BindWidget))
	UImage* IrisImage;

	// BP側のアニメーションと紐づける
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* IrisOpen;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* IrisClose;

	UFUNCTION(BlueprintCallable, Category="Iris")
	void StartIrisIn();

	UFUNCTION(BlueprintCallable, Category="Iris")
	void StartIrisOut();

public:

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnFadeAnimationFinished mOnFadeAnimationFinished;

protected:

	UFUNCTION()
	void BroadcastAnimationFinished();

protected:

	virtual void NativeConstruct() override;

protected:

	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:

	UPROPERTY()
	UMaterialInstanceDynamic* mIrisMID;
	
};
