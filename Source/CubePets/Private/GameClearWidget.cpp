// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearWidget.h"

void UGameClearWidget::StartFadeIn()
{
	if (FadeIn)
	{
		PlayAnimation(FadeIn);
	}
}
