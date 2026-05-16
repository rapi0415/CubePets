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

	// ˆÚ“®‘¬“x
	UPROPERTY(EditAnywhere, Category="Movement")
	float mMoveSpeed = 300.0f;

protected:

	// Œ©‚½–Ú
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> mStaticMesh = nullptr;

	// ƒRƒŠƒWƒ‡ƒ“
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USphereComponent> mSphereTrigger = nullptr;


};
