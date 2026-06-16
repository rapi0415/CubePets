// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CubePetsInputDeviceSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInputDeviceChanged, bool, bIsGamepad);
/**
 * 
 */
UCLASS()
class CUBEPETS_API UCubePetsInputDeviceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable, Category = "Input")
	FOnInputDeviceChanged mOnInputDeviceChanged;

	UFUNCTION(BlueprintCallable, Category = "Input")
	void NotifyDeviceChanged(bool bIsGamepad);

	UFUNCTION(BlueprintCallable, Category = "Input")
	bool IsUsingGamepad() const { return bIsUsingGamepad; }

private:

	bool bIsUsingGamepad = false;
	
};
