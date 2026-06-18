// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Subsystems/StageClearState.h"
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

	/*
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TArray<FName> mLevelNameArray;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	int32 mCurrentIndex = 0;
	*/

public:

	void OnIndexChanged(int32 Index, int32 MaxIndex, EStageClearState ClearState, bool MedalFlag, bool CubeFlag);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void BP_OnIndexChanged(int32 Index, int32 MaxIndex, EStageClearState ClearState, bool MedalFlag, bool CubeFlag);

	/*
public:

	UFUNCTION(BlueprintCallable, Category="UI")
	void ChangeIndex(int32 Direction);

	UFUNCTION(BlueprintCallable, Category="UI")
	void SelectCurrentLevel();

	*/

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

/*
protected:

	// アニメーション
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> DecideRightArrow;

public:

	// アニメーションを再生する関数
	UFUNCTION(BlueprintCallable, Category = "Animation")
	void StartDecideRightArrow();
	*/

};
