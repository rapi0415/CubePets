// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CubePetsGameModeBase.generated.h"

class USoundBase;
class UAudioComponent;

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACubePetsGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;

protected:

	// BGMçƒê∂óp
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Audio")
	TObjectPtr<USoundBase> mStageBGM = nullptr;

	UPROPERTY()
	TObjectPtr<UAudioComponent> mBGMAudioComponent = nullptr;

public:

	// BGMí‚é~ópä÷êî
	void StopStageBGM();

	
};
