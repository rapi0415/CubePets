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

// 数値に変換するタグを2つ持っているテキストを表示するときに使う
void UCSVTextBlock::UpdateTextWithTwoInts(FName NewTextID, int32 Value1, int32 Value2)
{
	if (DataTable && !NewTextID.IsNone())
	{
		CSVRowName = NewTextID;

		FCubePetsTextData* Row = DataTable->FindRow<FCubePetsTextData>(CSVRowName, TEXT(""));

		if (Row)
		{
			FText BaseText = Row->SystemText;
			
			FFormatOrderedArguments Args;

			// 使った箱の最高記録用の処理。100のときにハイフン（-）で表示したい（初回だけ100で以降は99でキャップされる） 
			if (Value1 == 100)
			{
				Args.Add(FText::FromString(TEXT("-")));
			}
			else
			{
				Args.Add(FText::AsNumber(Value1));
			}
			
			Args.Add(FText::AsNumber(Value2));

			SetText(FText::Format(BaseText, Args));
		}
	}
}
