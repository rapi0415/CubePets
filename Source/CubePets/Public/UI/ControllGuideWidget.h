// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControllGuideWidget.generated.h"

class UImage;
class UTexture2D;
class UCSVTextBlock;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UControllGuideWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	virtual void NativeConstruct() override;
	
public:

	// ジャンプボタン
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> JumpButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	TObjectPtr<UTexture2D> mJumpKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	TObjectPtr<UTexture2D> mJumpGamepadIcon = nullptr;

	// 生成ボタン
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> CreateButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	TObjectPtr<UTexture2D> mCreateKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	TObjectPtr<UTexture2D> mCreateGamepadIcon = nullptr;

	// 向き固定ボタン
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> LockRotButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	TObjectPtr<UTexture2D> mLockRotKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	TObjectPtr<UTexture2D> mLockRotGamepadIcon = nullptr;

	// 箱の生成ガイドテキスト
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UCSVTextBlock> TextBlockCreate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Text")
	FName mTextReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Text")
	FName mTextCreate;

	void UpdateTextBlockCreate(bool bIsReticleExistence);
	void UpdateDeviceIcon(bool bIsGamepad);

};
