// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsSelectPlayerController.generated.h"

class UStageSelectWidget;

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

	UPROPERTY(Transient)
	TObjectPtr<UStageSelectWidget> mCurrentStageSelectWidget = nullptr;
	
};
