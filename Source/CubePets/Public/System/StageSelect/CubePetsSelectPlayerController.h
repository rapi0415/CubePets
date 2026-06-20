// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CubePetsSelectPlayerController.generated.h"

class UStageSelectWidget;
class UIrisWidget;
class UInputAction;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ENextDestination : uint8
{
	NONE,
	TITLE,
	STAGE,
};

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsSelectPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

protected:

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UStageSelectWidget> mStageSelectWidgetClass = nullptr;

	UPROPERTY(EditAnywhere, Category="UI")
	TSubclassOf<UIrisWidget> mIrisWidgetClass = nullptr;

protected:

	UPROPERTY(Transient)
	TObjectPtr<UStageSelectWidget> mCurrentStageSelectWidget = nullptr;

	UPROPERTY(Transient)
	TObjectPtr<UIrisWidget> mCurrentIrisWidget = nullptr;

protected:

	// インプット系
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> mSelectMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mDecideAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mLeftRightAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mCancelAction = nullptr;

	// 決定ボタン関連
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressDecide();

	// 左右ボタン関連
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressLeftRight(const FInputActionValue& Value);

	// キャンセルボタン関連
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressCancel();

	virtual void SetupInputComponent() override;

protected:

	// ボタンアイコン切り替え用
	virtual bool InputKey(const FInputKeyParams& Params) override;

	bool bIsUsingGamepad = false;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	FName mTitleLevelName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Level")
	TArray<FName> mLevelNameArray;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int32 mCurrentIndex = 0;

public:

	UFUNCTION(BlueprintCallable, Category = "Level")
	void TransitionToTitle();

	UFUNCTION(BlueprintCallable, Category = "Level")
	void TransitionToStage();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeIndex(int32 Direction);

protected:

	UPROPERTY()
	ENextDestination mNextDestination = ENextDestination::NONE;

protected:

	UFUNCTION()
	void OnFinishIrisIn();

	UFUNCTION()
	void OnFinishIrisOut();

protected:

	// アイリスインが終わったかどうか
	bool bIsActiveInput = false;
	
};
