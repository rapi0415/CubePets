// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "CubePetsTitlePlayerController.generated.h"

class UTitleScreenWidget;
class UPreTitleWidget;
class UIrisWidget;
class UInputAction;
class UInputMappingContext;

UENUM(BlueprintType)
enum class ETitleState : uint8
{
	PRE_TITLE,
	MAIN_TITLE,
	NONE
};

UENUM(BlueprintType)
enum class ETitleMenuItem : uint8
{
	START,
	LOAD,
	JAPANESE,
	ENGLISH,
	SHOP,
	QUIT
};
/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsTitlePlayerController : public APlayerController
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

protected:

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
	TObjectPtr<UInputMappingContext> mTitleMappingContext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mConfirmAction = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> mUpDownAction = nullptr;

	// 決定ボタン関連
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressDecide();

	ETitleState mCurrentTitleState = ETitleState::PRE_TITLE;

	void HandlePreTitleDecide();
	void HandleMainTitleDecide();

	void OnMenuDecided(ETitleMenuItem ChosenItem);

	void StartGame();
	void LoadGame();
	void ChangeJapanese();
	void ChangeEnglish();
	void QuitGame();

	// 上下ボタン
	UFUNCTION(BlueprintCallable, Category = "Input")
	void OnPressUpDown(const FInputActionValue& Value);

	virtual void SetupInputComponent() override;

	bool bIsStarted = false;

protected:

	UFUNCTION()
	void WarpTo();

	FName mTargetLevelName;

protected:

	UFUNCTION()
	void ChangeTitleStateToMainTitle();

public:

	UFUNCTION(BlueprintCallable, Category = "UI")
	void ChangeIndex(int32 Direction);

protected:
	
	// メニュー操作用
	UPROPERTY(BlueprintReadOnly, Category = "UI")
	int32 mCurrentIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	int32 mMaxIndex = 5;

	TArray<int32> mSelectableIndices;

	int32 mCurrentPos = 0;

	// セーブデータの有無
	bool bHasSaveData = false;


protected:

	// ボタンアイコン切り替え用
	virtual bool InputKey(const FInputKeyParams& Params) override;

	bool bIsUsingGamepad = false;

public:

	UFUNCTION()
	void PlayDecorationPartsAnimation();



};
