// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ControllGuideWidget.h"
#include "UI/CSVTextBlock.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UControllGuideWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

// 箱の生成ガイドテキストの更新用
void UControllGuideWidget::UpdateTextBlockCreate(bool bIsReticleExistence)
{
	FName NewTextID = bIsReticleExistence ? mTextCreate : mTextReady;

	if (TextBlockCreate)
	{
		TextBlockCreate->UpdateTextID(NewTextID);
	}
}

