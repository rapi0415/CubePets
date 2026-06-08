// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"
#include "DebugMenuSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class CUBEPETS_API UDebugMenuSubsystem : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaTime) override;
	virtual TStatId GetStatId() const override;
	virtual bool IsTickable() const override;

protected:

	virtual bool ShouldCreateSubsystem(UObject* Outer) const override;

#if !UE_BUILD_SHIPPING
	void RenderPlayerMenu();

	void WarpToLocation(FVector TargetLocation);
	void RenderStageMenu();

	void RenderSystemMenu();
#endif
};
