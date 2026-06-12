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

	// 集めたメダルテキスト
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockMedalCount = nullptr;

	// 使った箱テキスト
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockCubeCount = nullptr;

public:

	// 集めたメダルテキスト更新用
	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateMedalText();

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void UpdateCubeText();

protected:

	virtual void NativeConstruct() override;
	
};
