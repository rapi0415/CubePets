// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"
#include "DoorBase.generated.h"

class UAbilitySystemComponent;
class UGameplayAbility;

UCLASS()
class CUBEPETS_API ADoorBase : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoorBase();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	// GASのコアコンポーネント（ASC）
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAbilitySystemComponent> mAbilitySystemComponent = nullptr;

	// ゲーム開始時に自動付与するアビリティ
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Abilities")
	TArray<TSubclassOf<UGameplayAbility>> mDefaultAbilities;

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Door")
	void OnOpenDoorTriggered();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category="Door")
	void OnCloseDoorTriggered();

};
