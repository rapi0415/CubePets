// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsTitlePlayerController.generated.h"

class UTitleScreenWidget;
class UPreTitleWidget;
class UIrisWidget;
class UInputAction;
class UInputMappingContext;

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

public:

	// タイトル画面用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UTitleScreenWidget> mTitleWidgetClass = nullptr;

	// プレタイトル画面画面用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UPreTitleWidget> mPreTitleWidgetClass = nullptr;

	// フェード演出用
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UIrisWidget> mIrisWidgetClass = nullptr;

protected:
	
	// タイトルウィジェット
	UPROPERTY(Transient)
	TObjectPtr<UTitleScreenWidget> mCurrentTitleWidget = nullptr;

	// プレタイトルウィジェット
	UPROPERTY(Transient)
	TObjectPtr<UPreTitleWidget> mCurrentPreTitleWidget = nullptr;

	// フェードウィジェット
	UPROPERTY(Transient)
	TObjectPtr<UIrisWidget> mCurrentIrisWidget = nullptr;

protected:

	// インプット系
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> mPreTitleMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mConfirmAction = nullptr;

	UFUNCTION(BlueprintCallable, Category = "Pause")
	void OnPressConfirm();

	virtual void SetupInputComponent() override;

	bool bIsStarted = false;

protected:

	// ボタンアイコン切り替え用
	virtual bool InputKey(const FInputKeyParams& Params) override;

	bool bIsUsingGamepad = false;

public:

	UFUNCTION()
	void PlayDecorationPartsAnimation();



};
