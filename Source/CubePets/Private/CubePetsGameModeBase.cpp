// Fill out your copyright notice in the Description page of Project Settings.


#include "CubePetsGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

void ACubePetsGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	if (mStageBGM)
	{
		mBGMAudioComponent = UGameplayStatics::SpawnSound2D(this, mStageBGM);
	}
}

void ACubePetsGameModeBase::StopStageBGM()
{
	if (mBGMAudioComponent && mBGMAudioComponent->IsPlaying())
	{
		mBGMAudioComponent->FadeOut(1.0f, 0.0f);
	}
}
