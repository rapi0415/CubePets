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
	UImage* JumpButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	UTexture2D* mJumpKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	UTexture2D* mJumpGamepadIcon = nullptr;

	// 生成ボタン
	UPROPERTY(meta = (BindWidget))
	UImage* CreateButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	UTexture2D* mCreateKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	UTexture2D* mCreateGamepadIcon;

	// 向き固定ボタン
	UPROPERTY(meta = (BindWidget))
	UImage* LockRotButtonIconImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	UTexture2D* mLockRotKeyboardIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Icons")
	UTexture2D* mLockRotGamepadIcon = nullptr;

	// 箱の生成ガイドテキスト
	UPROPERTY(meta = (BindWidget))
	UCSVTextBlock* TextBlockCreate = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Text")
	FName mTextReady;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI|Text")
	FName mTextCreate;

	void UpdateTextBlockCreate(bool bIsReticleExistence);

	void UpdateDeviceIcon(bool bIsGamepad);

};
