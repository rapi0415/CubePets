// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StageSelectWidget.generated.h"

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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TArray<FName> mLevelNameArray;

	UPROPERTY(BlueprintReadOnly, Category="UI")
	int32 mCurrentIndex = 0;

protected:

	void OnIndexChanged(UTexture2D* NewTexture);

	UFUNCTION(BlueprintImplementableEvent, Category="UI")
	void BP_OnIndexChanged(UTexture2D* NewTexture);

public:

	UFUNCTION(BlueprintCallable, Category="UI")
	void ChangeIndex(int32 Direction);

	UFUNCTION(BlueprintCallable, Category="UI")
	void SelectCurrentLevel();

};
