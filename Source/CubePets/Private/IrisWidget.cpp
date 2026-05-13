// Fill out your copyright notice in the Description page of Project Settings.


#include "IrisWidget.h"
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
		PlayAnimation(IrisOpen);
		UE_LOG(LogTemp, Warning, TEXT("Play IrisOpen"));
	}
}

void UIrisWidget::StartIrisOut()
{
	if (IrisClose)
	{
		// アニメーションが終わったら通知用の関数を呼ぶためにバインド
		FWidgetAnimationDynamicEvent EndDelegate;
		EndDelegate.BindDynamic(this, &UIrisWidget::BroadcastAnimationFinished);

		UnbindFromAnimationFinished(IrisClose, EndDelegate);
		BindToAnimationFinished(IrisClose, EndDelegate);

		// アニメーション再生
		PlayAnimation(IrisClose);
	}
}

void UIrisWidget::BroadcastAnimationFinished()
{
	mOnFadeAnimationFinished.Broadcast();
}
