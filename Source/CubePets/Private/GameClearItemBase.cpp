// Fill out your copyright notice in the Description page of Project Settings.


#include "GameClearItemBase.h"
#include "CubePetsCharacter.h"
#include "CubePetsPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "GameClearWidget.h"

void AGameClearItemBase::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool FromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor && OtherActor != this && !bIsCleared)
	{
		// 接触したのがプレイヤーか？
		if (ACubePetsCharacter* PlayerCharacter = Cast<ACubePetsCharacter>(OtherActor))
		{
			bIsCleared = true;

			// 入力を無効化する
			if (ACubePetsPlayerController* PC = Cast<ACubePetsPlayerController>(PlayerCharacter->GetController()))
			{
				PlayerCharacter->DisableInput(PC);
				PC->DisableInput(PC);
			}

			// メッシュを非表示
			if (mStaticMesh)
			{
				mStaticMesh->SetVisibility(false);
			}

			// 一定時間後にゲームクリアUIを表示
			float DelayTimeUI = 2.0f;
			GetWorld()->GetTimerManager().SetTimer(mClearTimerHandle, this, &AGameClearItemBase::ShowGameClearUI, DelayTimeUI, false);

			// さらに一定時間後にフェードアウトしてタイトルに戻る
			float DelayTimeFade = 4.0f;
			GetWorld()->GetTimerManager().SetTimer(mFadeOutTimerHandle, this, &AGameClearItemBase::StartFadeOut, DelayTimeFade, false);
		}
	}
}

void AGameClearItemBase::ShowGameClearUI()
{
	if (mGameClearWidgetClass)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (PC)
		{
			mCurrentGameClearWidget = CreateWidget<UGameClearWidget>(PC, mGameClearWidgetClass);

			if (mCurrentGameClearWidget)
			{
				mCurrentGameClearWidget->AddToViewport(10);
				mCurrentGameClearWidget->StartFadeIn();
			}
		}

	}

}

void AGameClearItemBase::StartFadeOut()
{
	ACubePetsPlayerController* PC = Cast<ACubePetsPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));

	if (PC && !mTargetLevel.IsNull())
	{
		FName LevelName = FName(*mTargetLevel.GetAssetName());
		PC->RequestLevelTransition(LevelName);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("遷移先のレベルが設定されていないか、PCの取得に失敗しました"));
	}
}
