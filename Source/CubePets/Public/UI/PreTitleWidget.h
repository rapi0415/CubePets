// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreTitleWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UPreTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	// 開始ボタン
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ConfirmButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Icons")
	TObjectPtr<UTexture2D> mConfirmKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI|Icons")
	TObjectPtr<UTexture2D> mConfirmGamepadIcon = nullptr;

public:

	void UpdateDeviceIcon(bool bIsGamepad);

};
