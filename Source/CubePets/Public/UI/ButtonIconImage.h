// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonIconImage.generated.h"

class UImage;
/**
 * 
 */
UCLASS()
class CUBEPETS_API UButtonIconImage : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> IconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Icons")
	TObjectPtr<UTexture2D> mKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI|Icons")
	TObjectPtr<UTexture2D> mGamepadIcon = nullptr;

	UFUNCTION(BlueprintCallable, Category = "UI|Icons")
	void UpdateDeviceIcon(bool bIsGamepad);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


protected:

	UFUNCTION()
	void OnDeviceChanged(bool bIsGamepad);
};
