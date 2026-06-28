// Fill out your copyright notice in the Description page of Project Settings.


#include "System/Title/CubePetsTtileModeBase.h"

void ACubePetsTtileModeBase::BeginPlay()
{
	Super::BeginPlay();
}

void ACubePetsTtileModeBase::StopTitleBGM()
{
	BP_StopTitleBGM();
}

void ACubePetsTtileModeBase::PlayMusic()
{
	BP_PlayMusic();
}

