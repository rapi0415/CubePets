// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "CubePetsCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class ACubeReticle;
class ACubePetsCube;

// フェードアウト処理用のデリゲート
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFadeOutTriggered);

UCLASS()
class CUBEPETS_API ACubePetsCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACubePetsCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// カメラ用
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<USpringArmComponent> mCameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera")
	TObjectPtr<UCameraComponent> mFollowCamera;

protected:
	
	// 入力用
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputMappingContext> mDefaultMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mMoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mLookAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mJumpAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mCreateAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	TObjectPtr<UInputAction> mLockRotationAction;

protected:

	// ジャンプ制御用
	bool bIsJumpButtonReleased = true;

protected:

	// ブロック生成用
	UPROPERTY()
	TObjectPtr<ACubeReticle> mCubeReticle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="CubePets")
	TSubclassOf<ACubeReticle> mReticleClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CubePets")
	float mSpawnOffset = 150.0f;

	bool bIsReticleExistence = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="CubePets")
	int32 mMaxCube = 5;

	UPROPERTY()
	TArray<TObjectPtr<ACubePetsCube>> mCubePetsCubeArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CubePets")
	TSubclassOf<ACubePetsCube> mCubePetsCubeClass = nullptr;

	int32 mNextIndex = 0;

	UPROPERTY()
	TObjectPtr<AActor> mLastPlayerBase = nullptr;

protected:

	// ブロック生成用
	UFUNCTION()
	void UpdateOldestCubeGlow();

protected:

	// ダメージ処理用
	virtual float TakeDamage
	(
		float DamageAmount, 
		struct FDamageEvent const& DamageEvent, 
		class AController* EventInstigator, 
		AActor* DamageCauser
	) override;

	bool bIsDead = false;

	FTimerHandle mRestartTimerHandle;

	UPROPERTY(EditAnywhere, Category="GameRules")
	float mRestartDelay = 1.0f;

	void BroadcastFadeOut();

public:

	UPROPERTY(BlueprintAssignable, Category="Events")
	FOnFadeOutTriggered mOnFadeOutTriggered;

protected:

	// InputAction
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void JumpAction(const FInputActionValue& Value);
	void StopJumpAction(const FInputActionValue& Value);
	void CreateAction(const FInputActionValue& Value);
	void LockRotation(const FInputActionValue& Value);
	void UnlockRotation(const FInputActionValue& Value);

};
