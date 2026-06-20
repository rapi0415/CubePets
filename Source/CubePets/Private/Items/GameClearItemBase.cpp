// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/GameClearItemBase.h"
#include "Characters/CubePetsCharacter.h"
#include "UI/GameClearWidget.h"
#include "System/InGame/CubePetsPlayerController.h"
#include "System/InGame/CubePetsGameModeBase.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "Kismet/GameplayStatics.h"

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
			Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, FromSweep, SweepResult);

			bIsCleared = true;

			// 入力を無効化する
			if (ACubePetsPlayerController* PC = Cast<ACubePetsPlayerController>(PlayerCharacter->GetController()))
			{
				PlayerCharacter->DisableInput(PC);
				PC->DisableInput(PC);
			}

			// 一定時間後にゲームクリアUIを表示
			float DelayTimeUI = 2.0f;
			GetWorld()->GetTimerManager().SetTimer(mClearTimerHandle, this, &AGameClearItemBase::ShowGameClearUI, DelayTimeUI, false);

			// さらに一定時間後にフェードアウトしてステージ遷移
			float DelayTimeFade = 4.0f;
			GetWorld()->GetTimerManager().SetTimer(mFadeOutTimerHandle, this, &AGameClearItemBase::StartFadeOut, DelayTimeFade, false);

			// BGMを止める
			if (UWorld* World = GetWorld())
			{
				if (ACubePetsGameModeBase* GM = Cast<ACubePetsGameModeBase>(World->GetAuthGameMode()))
				{
					GM->StopStageBGM();
				}
			}

			// ステージ情報更新
			UGameInstance* GameInstance = GetGameInstance();
			if (GameInstance)
			{
				UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
				if (ProgressionSubsystem)
				{
					// ステージをクリア済みにして次のステージを解放
					ProgressionSubsystem->UnLockNextStage(mStageIndex);

					// 使った箱の最高記録を更新
					ProgressionSubsystem->UpdateRecordCubeCount();

					// ステージをクリア済みにする（完全クリアかどうかはSubsystem側で判定してもらう）
					ProgressionSubsystem->UpdateCurrentStageClearState(mStageIndex);
				}
			}
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

