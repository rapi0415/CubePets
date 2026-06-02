// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/CSVTextBlock.h"

void UCSVTextBlock::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	if (DataTable && !CSVRowName.IsNone())
	{
		FCubePetsTextData* Row = DataTable->FindRow<FCubePetsTextData>(CSVRowName, TEXT(""));

		if (Row)
		{
			SetText(Row->SystemText);
		}
	}
}

// 動的に表示するテキストIDを変えたいときに使う
void UCSVTextBlock::UpdateTextID(FName NewTextID)
{
	if (DataTable && !CSVRowName.IsNone())
	{
		CSVRowName = NewTextID;

		FCubePetsTextData* Row = DataTable->FindRow<FCubePetsTextData>(CSVRowName, TEXT(""));

		if (Row)
		{
			SetText(Row->SystemText);
		}
	}
}
