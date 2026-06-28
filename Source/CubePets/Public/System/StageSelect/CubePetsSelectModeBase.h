// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CubePetsSelectModeBase.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsSelectModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
protected:

	virtual void BeginPlay() override;

protected:

	// BGM再生用コンポーネント
	UPROPERTY(BlueprintReadWrite, Category = "Audio")
	TObjectPtr<UAudioComponent> mBGMAudioComponent = nullptr;

public:

	void StopBGM();

protected:

	// BGM停止用関数
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Audio")
	void BP_StopBGM();

};
