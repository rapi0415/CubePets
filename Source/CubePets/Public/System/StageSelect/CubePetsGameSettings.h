// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "Engine/DataTable.h"
#include "CubePetsGameSettings.generated.h"

/**
 * 
 */
UCLASS(Config=Game, defaultconfig, meta=(DisplayName="CubePets Game Settings"))
class CUBEPETS_API UCubePetsGameSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:

	// データテーブルをセットするためのプロパティ
	UPROPERTY(Config, EditAnywhere, BlueprintReadOnly, Category="Medal")
	FSoftObjectPath mMedalDataTablePath;
};
