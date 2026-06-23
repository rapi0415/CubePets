// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PauseWidget.h"

void UPauseWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OnIndexChanged(0);
}

void UPauseWidget::OnIndexChanged(int32 Index)
{
	BP_OnIndexChanged(Index);
}

