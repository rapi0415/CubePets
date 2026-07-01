// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SwitchBase.generated.h"

class UBoxComponent;
class UMaterialInstanceDynamic;

UCLASS()
class CUBEPETS_API ASwitchBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitchBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// スイッチの見た目
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> mStaticMesh = nullptr;

	// TriggerBox
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<UBoxComponent> mBoxTrigger = nullptr;

protected:

	UFUNCTION()
	void OnOverlapBegin(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	UFUNCTION(BlueprintImplementableEvent, Category="Switch")
	void OnSwitchActivated(AActor* Activator, AActor* Target);

	UFUNCTION()
	void OnOverlapEnd(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UFUNCTION(BlueprintImplementableEvent, Category="Switch")
	void OnSwitchDeactivated(AActor* Deactivator, AActor* Target);

	int32 mOverlapCount = 0;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Switch")
	TObjectPtr<AActor> mTargetActor = nullptr;

protected:

	UPROPERTY(Transient)
	TObjectPtr<UMaterialInstanceDynamic> mDynamicMaterial = nullptr;
};
