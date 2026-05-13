// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

class UButton;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartButtonPressed);

/**
 * 
 */
UCLASS()
class CUBEPETS_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:

	// virtual void NativeConstruct() override;

public:

	/*
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="Events")
	FOnStartButtonPressed mOnStartButtonPressed;
	*/

public:

	// BP‘¤‚Ìƒ{ƒ^ƒ“‚Æ•R‚Ã‚¯‚é
	// UPROPERTY(Transient, meta = (BindWidget), BlueprintReadOnly)
	// UButton* StartButton;

public:

	// void SetupInitialFocus();

};
