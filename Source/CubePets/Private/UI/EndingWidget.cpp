// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EndingWidget.h"

void UEndingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	StartAppearanceText();

	mOnAppearanceAnimationFinished.AddDynamic(this, &UEndingWidget::ShowButton);
}

void UEndingWidget::StartAppearanceText()
{
	if (AppearanceText)
	{
		// アニメーションが終わったら通知用の関数を呼ぶためにバインド
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UEndingWidget::OnAppearanceAnimationFinished);

		UnbindFromAnimationFinished(AppearanceText, EndDelegate);
		BindToAnimationFinished(AppearanceText, EndDelegate);

		PlayAnimation(AppearanceText);
	}
}

void UEndingWidget::OnAppearanceAnimationFinished()
{
	mOnAppearanceAnimationFinished.Broadcast();
}

void UEndingWidget::ShowButton()
{
	BP_ShowButton();
}
