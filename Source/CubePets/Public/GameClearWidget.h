// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameClearWidget.generated.h"

class UWidgetAnimation;

/**
 * 
 */
UCLASS()
class CUBEPETS_API UGameClearWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	TObjectPtr<UWidgetAnimation> FadeIn;
	
public:

	UFUNCTION()
	void StartFadeIn();

};
