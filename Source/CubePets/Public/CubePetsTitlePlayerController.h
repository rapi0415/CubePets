// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "CubePetsTitlePlayerController.generated.h"

class UTitleWidget;

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
	TSubclassOf<UTitleWidget> mTitleWidgetClass = nullptr;
	
	UPROPERTY()
	UTitleWidget* mCurrentTitleWidget = nullptr;

protected:

	UPROPERTY(EditAnywhere, Category="Level Transition")
	TSoftObjectPtr<UWorld> mStartLevel = nullptr;

protected:

	UFUNCTION()
	void HandleGameStartNotification();

};
