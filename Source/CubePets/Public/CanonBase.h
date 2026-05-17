// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "CanonBase.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACanonBase : public AGimmickBase
{
	GENERATED_BODY()

public:

	ACanonBase();

protected:

	virtual void BeginPlay() override;

protected:

	FTimerHandle mSearchTimerHandle;

	void SearchForPlayer();

	// プレイヤーを検知したときに呼び出すイベント（BP側でスポーン位置など決める）
	UFUNCTION(BlueprintImplementableEvent, Category="Canon")
	void OnPlayerDetected();

protected:

	// StaticMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* mStaticMesh = nullptr;

	// BoxTrigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent* mBoxTrigger = nullptr;
	
};
