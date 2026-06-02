// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CubePetsTtileModeBase.generated.h"

class USoundBase;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsTtileModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

protected:

	// BGM再生用コンポーネント
	UPROPERTY(BlueprintReadWrite, Category="Audio")
	TObjectPtr<UAudioComponent> mBGMAudioComponent = nullptr;

public:

	// BGM停止用関数
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Audio")
	void StopTitleBGM();
};
