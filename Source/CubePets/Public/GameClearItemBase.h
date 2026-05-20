// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickupItemBase.h"
#include "GameClearItemBase.generated.h"

class UGameClearWidget;

/**
 * 
 */
UCLASS()
class CUBEPETS_API AGameClearItemBase : public APickupItemBase
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

	FTimerHandle mClearTimerHandle;
	FTimerHandle mFadeOutTimerHandle;

	bool bIsCleared = false;

protected:

	// UI関連
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TObjectPtr<UGameClearWidget> mCurrentGameClearWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="UI")
	TSubclassOf<UUserWidget> mGameClearWidgetClass = nullptr;

protected:

	// クリア後に遷移するレベル
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="LevelTransition")
	TSoftObjectPtr<UWorld> mTargetLevel = nullptr;

protected:

	void ShowGameClearUI();
	void StartFadeOut();

};
