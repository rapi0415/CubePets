// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageStatusWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "UI/CSVTextBlock.h"

void UStageStatusWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateMedalText();

	// Subsystemのイベントをバインドする（メダルテキストを更新するため）
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			ProgressionSubsystem->OnMedalCountChanged.AddDynamic(this, &UStageStatusWidget::UpdateMedalText);
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