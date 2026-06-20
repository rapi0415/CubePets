// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsEndingPlayerController.generated.h"

class UEndingWidget;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsEndingPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

protected:

	// ウィジェット
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UEndingWidget> mEndingWidgetClass = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UEndingWidget> mCurrentEndingWidget = nullptr;

protected:

	// インプット系
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> mEndingMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mDecideAction = nullptr;

	virtual void SetupInputComponent() override;

	// 決定ボタン
	UFUNCTION()
	void OnPressDecide();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	FName mTitleLevelName;

protected:

	bool bIsActive = false;

	UFUNCTION()
	void ActiveInput();

protected:

	// ボタンアイコン切り替え用
	virtual bool InputKey(const FInputKeyParams& Params) override;

	
};
