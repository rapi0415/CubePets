// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUDWidget.h"
#include "UI/ControllGuideWidget.h"

void UGameHUDWidget::UpdateTextBlockCreate(bool bIsReticleExistence)
{
	if (ControlGuide)
	{
		UControllGuideWidget* Control = Cast<UControllGuideWidget>(ControlGuide);
		if (Control)
		{
			Control->UpdateTextBlockCreate(bIsReticleExistence);
		}
	}
}