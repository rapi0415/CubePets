// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GimmickCannonInterfase.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UGimmickCannonInterfase : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class CUBEPETS_API IGimmickCannonInterfase
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, Category="Gimmick")
	virtual FTransform GetBulletSpawnPointTransform() const
	{
		return {};
	}

};
