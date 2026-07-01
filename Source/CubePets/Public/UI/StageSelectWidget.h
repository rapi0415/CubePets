// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Enums/StageClearState.h"
#include "StageSelectWidget.generated.h"

class UCSVTextBlock;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UStageSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TArray<TSoftObjectPtr<UTexture2D>> mImageArray;

public:

	void OnIndexChanged(int32 Index, int32 MaxIndex, EStageClearState ClearState, bool MedalFlag, bool CubeFlag);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void BP_OnIndexChanged(int32 Index, int32 MaxIndex, EStageClearState ClearState, bool MedalFlag, bool CubeFlag);


protected:

	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockStageNum = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI|Text")
	TArray<FName> mStageNumTextArray;

protected:

	// テキストブロック（集めたメダル）
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockMedalCount = nullptr;

	// テキストブロック（使った箱）
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockCubeCount = nullptr;

public:

	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateMedalText();

	UFUNCTION(BlueprintCallable, Category="UI")
	void UpdateCubeText();

protected:

	virtual void NativeConstruct() override;

};
