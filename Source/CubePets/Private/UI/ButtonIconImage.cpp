// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ButtonIconImage.h"
#include "Components/Image.h"
#include "Subsystems/CubePetsInputDeviceSubsystem.h"

void UButtonIconImage::UpdateDeviceIcon(bool bIsGamepad)
{
	UTexture2D* NewIcon = bIsGamepad ? mGamepadIcon : mKeyboardIcon;

	if (NewIcon)
	{
		if (IconImage)
		{
			IconImage->SetBrushFromTexture(NewIcon);
		}
	}
}

void UButtonIconImage::NativeConstruct()
{
	Super::NativeConstruct();

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UCubePetsInputDeviceSubsystem* Subsystem = GameInstance->GetSubsystem<UCubePetsInputDeviceSubsystem>();
		if (Subsystem)
		{
			Subsystem->mOnInputDeviceChanged.AddDynamic(this, &UButtonIconImage::OnDeviceChanged);
			UpdateDeviceIcon(Subsystem->IsUsingGamepad());
		}
	}
}

void UButtonIconImage::NativeDestruct()
{
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UCubePetsInputDeviceSubsystem* Subsystem = GameInstance->GetSubsystem<UCubePetsInputDeviceSubsystem>();
		if (Subsystem)
		{
			Subsystem->mOnInputDeviceChanged.RemoveDynamic(this, &UButtonIconImage::OnDeviceChanged);
		}
	}
	Super::NativeDestruct();
}

void UButtonIconImage::OnDeviceChanged(bool bIsGamepad)
{
	UpdateDeviceIcon(bIsGamepad);
}
