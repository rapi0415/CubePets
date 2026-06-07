// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GimmickCannonInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(NotBlueprintable)
class UGimmickCannonInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * こんな感じにインターフェースにするとキャストが減って依存関係が複雑にならない
 */
class CUBEPETS_API IGimmickCannonInterface
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable)
	virtual FTransform GetBulletSpawnPointTransform() const
	{
		return {};
	}

};
