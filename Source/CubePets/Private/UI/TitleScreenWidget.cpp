// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleScreenWidget.h"

void UTitleScreenWidget::StartAppearanceDecoration()
{
	if (AppearanceDecoration)
	{
		// アニメーションが終わったら通知用の関数を呼ぶためにバインド
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UTitleScreenWidget::OnTitleAnimationFinished);

		UnbindFromAnimationFinished(AppearanceDecoration, EndDelegate);
		BindToAnimationFinished(AppearanceDecoration, EndDelegate);

		PlayAnimation(AppearanceDecoration);
	}
}

void UTitleScreenWidget::SetIMC()
{
	BP_SetIMC();
}

void UTitleScreenWidget::OnTitleAnimationFinished()
{
	// アニメーションが終わったらIMCをバインド
	SetIMC();

	// テキストのフォーカスも行いたいのでOnIndexChangedを呼び出す
	OnIndexChanged();
}

void UTitleScreenWidget::OnIndexChanged()
{
	BP_OnIndexChanged();
}

void UTitleScreenWidget::ChangeIndex(int32 Direction)
{
	int32 TargetIndex = mCurrentIndex + Direction;

	if (TargetIndex > mMaxIndex)
	{
		return;
	}

	if (TargetIndex < 0)
	{
		return;
	}

	mCurrentIndex = TargetIndex;

	OnIndexChanged();
}


