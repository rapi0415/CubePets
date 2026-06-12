// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageStatusWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "UI/CSVTextBlock.h"

void UStageStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateMedalText();
	UpdateCubeText();

	// Subsystemのイベントをバインドする（テキストを更新するため）
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			ProgressionSubsystem->OnMedalCountChanged.AddDynamic(this, &UStageStatusWidget::UpdateMedalText);
			ProgressionSubsystem->OnUsedCubeCountChanged.AddDynamic(this, &UStageStatusWidget::UpdateCubeText);
		}
	}
}

// 集めたメダル用テキストの更新用
void UStageStatusWidget::UpdateMedalText()
{
	if (!TextBlockMedalCount) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Value1 = ProgressionSubsystem->GetCurrentMedalCount();
			int32 Value2 = ProgressionSubsystem->GetTotalMedal();

			TextBlockMedalCount->UpdateTextWithTwoInts(TEXT("Select_CollectMedal"), Value1, Value2);
		}
	}
}

// 使った箱テキストの更新用
void UStageStatusWidget::UpdateCubeText()
{
	if (!TextBlockCubeCount) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Value1 = ProgressionSubsystem->GetCurrentUsedCubeCount();
			int32 Value2 = ProgressionSubsystem->GetTotalCube();

			TextBlockCubeCount->UpdateTextWithTwoInts(TEXT("Select_UsedCubes"), Value1, Value2);
		}
	}
}
