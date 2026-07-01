// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/TitleScreenWidget.h"
#include "Subsystems/GameProgressionSubsystem.h"

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// セーブデータ存在確認
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* Subsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (Subsystem)
		{
			bHasSaveData = Subsystem->IsExistenceSaveData();
		}
	}

	// テキストのフォーカスも行いたいのでOnIndexChangedを呼び出す
	OnIndexChanged(0);
}

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

void UTitleScreenWidget::StartTextStartConfirmed()
{
	if (TextStartConfirmed)
	{
		// テキストの点滅アニメーション（ループ再生）
		PlayAnimation(TextStartConfirmed, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f, false);
	}
}

void UTitleScreenWidget::StartTextLoadConfirmed()
{
	if (TextLoadConfirmed)
	{
		// テキストの点滅アニメーション（ループ再生）
		PlayAnimation(TextLoadConfirmed, 0.0f, 0, EUMGSequencePlayMode::Forward, 1.0f, false);
	}
}

void UTitleScreenWidget::OnTitleAnimationFinished()
{
	// テキストのフォーカスも行いたいのでOnIndexChangedを呼び出す
	// OnIndexChanged(0);

	// バインドしている関数を呼ぶ（TitlePlayerControllerの状態遷移）
	mOnAnimationFinished.Broadcast();
}

void UTitleScreenWidget::OnIndexChanged(int32 Index)
{
	BP_OnIndexChanged(Index);
}





