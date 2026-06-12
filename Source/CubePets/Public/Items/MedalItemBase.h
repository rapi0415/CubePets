// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/PickupItemBase.h"
#include "MedalItemBase.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API AMedalItemBase : public APickupItemBase
{
	GENERATED_BODY()
	
protected:

	virtual void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	) override;

protected:

	bool bIsPickuped = false;

};
