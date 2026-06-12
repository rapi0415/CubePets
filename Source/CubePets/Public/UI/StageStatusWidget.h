// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageStatusWidget.generated.h"

class UCSVTextBlock;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UStageStatusWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockMedalCount = nullptr;

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateMedalText();

protected:

	virtual void NativeConstruct() override;
	
};
