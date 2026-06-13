// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/IrisWidget.h"
#include "Components/Image.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Animation/WidgetAnimationEvents.h"

void UIrisWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UIrisWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// ダイナミックマテリアルを作成
	if (IrisImage)
	{
		mIrisMID = IrisImage->GetDynamicMaterial();
	}
}

void UIrisWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	// AspectRatioを計算
	// Initializedのタイミングに計算だとウィンドウサイズが上手く取得できない場合があるのでTickで計算する（途中でウィンドウサイズを変えても大丈夫なようにする意図もある）
	FVector2D LocalSize = MyGeometry.GetLocalSize();

	if (LocalSize.X > 0 && LocalSize.Y > 0)
	{
		float AspectRatio = LocalSize.X / LocalSize.Y;
		if (mIrisMID)
		{
			mIrisMID->SetScalarParameterValue(TEXT("AspectRatio"), AspectRatio);
		}
	}
}

// アイリスインのアニメーションを再生するための関数
void UIrisWidget::StartIrisIn()
{
	if (IrisOpen)
	{
		// アニメーションが終わったら通知用の関数を呼ぶためにバインド
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UIrisWidget::OnIrisInAnimationFinished);

		UnbindFromAnimationFinished(IrisOpen, EndDelegate);
		BindToAnimationFinished(IrisOpen, EndDelegate);

		PlayAnimation(IrisOpen);
	}
}

void UIrisWidget::StartIrisOut()
{
	if (IrisClose)
	{
		// アニメーションが終わったら通知用の関数を呼ぶためにバインド
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UIrisWidget::OnIrisOutAnimationFinished);

		UnbindFromAnimationFinished(IrisClose, EndDelegate);
		BindToAnimationFinished(IrisClose, EndDelegate);

		PlayAnimation(IrisClose);
	}
}

void UIrisWidget::OnIrisOutAnimationFinished()
{
	mOnIrisOutFinished.Broadcast();
}

void UIrisWidget::OnIrisInAnimationFinished()
{
	mOnIrisInFinished.Broadcast();
}
