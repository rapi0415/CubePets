// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GimmickBase.h"
#include "GimmickCannonInterfase.h"
#include "CanonBase.generated.h"

class UBoxComponent;

/**
 * 
 */
UCLASS()
class CUBEPETS_API ACanonBase : public AGimmickBase, public IGimmickCannonInterfase
{
	GENERATED_BODY()

public:

	ACanonBase();

protected:

	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	virtual FTransform GetBulletSpawnPointTransform() const override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category="Gimmick", DisplayName="BP_GetBulletSpawnPointTransform")
	FTransform BP_GetBulletSpawnPointTransform() const;

protected:

	FTimerHandle mSearchTimerHandle;

	void SearchForPlayer();

	// プレイヤーを検知したときに呼び出すイベント（BP側でスポーン位置など決める）
	UFUNCTION(BlueprintImplementableEvent, Category="Canon")
	void OnPlayerDetected();

protected:

	// StaticMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> mMeshBase = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> mMeshTurret = nullptr;

	// BoxTrigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UBoxComponent> mBoxTrigger = nullptr;
	
};
