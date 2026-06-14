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
	TObjectPtr<UInputAction> mConfirmAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mLeftRightAction = nullptr;

	// 決定ボタン関連
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressConfirm();

	// 左右ボタン関連
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressLeftRight(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TArray<FName> mLevelNameArray;

	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int32 mCurrentIndex = 0;

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void OpenCurrentLevel();

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeIndex(int32 Direction);

protected:

	UFUNCTION()
	void OnFinishIrisIn();

protected:

	// アイリスインが終わったかどうか
	bool bIsIrisInFinished = false;
	
};
