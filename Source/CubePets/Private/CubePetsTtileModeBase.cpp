// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePetsTtileModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void ACubePetsTtileModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mTitleBGM)
	{
		mBGMAudioComponent = UGameplayStatics::SpawnSound2D(this, mTitleBGM);
	}
}

void ACubePetsTtileModeBase::StopTitleBGM()
{
	if (mBGMAudioComponent && mBGMAudioComponent->IsPlaying())
	{
		mBGMAudioComponent->FadeOut(1.0f, 0.0f);
	}
}
