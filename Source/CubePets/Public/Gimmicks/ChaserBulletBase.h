// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChaserBulletBase.generated.h"

class USphereComponent;

UCLASS()
class CUBEPETS_API AChaserBulletBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChaserBulletBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// 移動速度
	UPROPERTY(EditAnywhere, Category="Movement")
	float mMoveSpeed = 300.0f;

protected:

	// 見た目
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> mStaticMesh = nullptr;

	// コリジョン
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> mSphereTrigger = nullptr;

	// 誰が撃ったか記憶する変数
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Canon", meta = (ExposeOnSpawn = "true"))
	TObjectPtr<AActor> mShooterActor = nullptr;

protected:

	// 接触時に呼ばれる関数
	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintImplementableEvent, Category="Effects")
	void PlayEffects();

};
