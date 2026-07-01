// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/StageSelectWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "UI/CSVTextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "UI/StageStatusWidget.h"

void UStageSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UpdateMedalText();
	UpdateCubeText();
}

void UStageSelectWidget::OnIndexChanged(int32 Index, int32 MaxIndex, EStageClearState ClearState, bool MedalFlag, bool CubeFlag)
{
	BP_OnIndexChanged(Index, MaxIndex, ClearState, MedalFlag, CubeFlag);
}

void UStageSelectWidget::UpdateMedalText()
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

void UStageSelectWidget::UpdateCubeText()
{
	if (!TextBlockCubeCount) return;

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			int32 Value1 = ProgressionSubsystem->GetRecordCubeCount();
			int32 Value2 = ProgressionSubsystem->GetTotalCube();

			TextBlockCubeCount->UpdateTextWithTwoInts(TEXT("Select_UsedCubes"), Value1, Value2);
		}
	}
}

