// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CubePetsInputDeviceSubsystem.h"

void UCubePetsInputDeviceSubsystem::NotifyDeviceChanged(bool bIsGamepad)
{
	// 変化が無ければスキップ
	if (bIsGamepad == bIsUsingGamepad) return;

	bIsUsingGamepad = bIsGamepad;
	mOnInputDeviceChanged.Broadcast(bIsGamepad);
}
