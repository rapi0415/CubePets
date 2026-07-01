// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControllGuideWidget.generated.h"

class UImage;
class UTexture2D;
class UCSVTextBlock;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UControllGuideWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
public:

	// 箱の生成ガイドテキスト
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockCreate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Text")
	FName mTextReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Text")
	FName mTextCreate;

	void UpdateTextBlockCreate(bool bIsReticleExistence);

};
