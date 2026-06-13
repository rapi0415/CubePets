// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/PreTitleWidget.h"
#include "Components/Image.h"

// ボタンアイコンをキーボード用とパッド用で切り替える
void UPreTitleWidget::UpdateDeviceIcon(bool bIsGamepad)
{
	if (ConfirmButtonIconImage)
	{
		UTexture2D* NewConfirmIcon = bIsGamepad ? mConfirmGamepadIcon : mConfirmKeyboardIcon;

		if (NewConfirmIcon)
		{
			ConfirmButtonIconImage->SetBrushFromTexture(NewConfirmIcon);
		}
	}
}