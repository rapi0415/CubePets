// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CubeReticle.generated.h"

class UBoxComponent;
class UMaterialInstanceDynamic;

UCLASS()
class CUBEPETS_API ACubeReticle : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACubeReticle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 生成許可か禁止かを返す関数
	virtual bool GetIsPermission()
	{
		return bIsPermission;
	}

protected:

	// StaticMesh
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UStaticMeshComponent* mStaticMesh = nullptr;

	// BoxTrigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	UBoxComponent* mBoxTrigger = nullptr;

	// DynamicMaterialInstance
	UPROPERTY()
	UMaterialInstanceDynamic* mDynamicMaterial = nullptr;

	// レティクルの状態（生成許可か禁止か）
	bool bIsPermission = true;

};
