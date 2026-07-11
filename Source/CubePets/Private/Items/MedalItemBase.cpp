// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MedalItemBase.h"
#include "Characters/CubePetsCharacter.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "System/InGame/CubePetsPlayerController.h"

void AMedalItemBase::BeginPlay()
{
	Super::BeginPlay();

	// Subsystemを取得
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			// 獲得済みかどうか調べて、獲得済みなら消す
			if (ProgressionSubsystem->IsMedalAlreadyCollected(mMedalID))
			{
				Destroy();
				return;
			}
		}
	}
}

void AMedalItemBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AMedalItemBase::OnOverlapBegin
(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (OtherActor && OtherActor != this && !bIsPickuped)
	{
		// 接触したのがプレイヤーか？
		if (ACubePetsCharacter* PlayerCharacter = Cast<ACubePetsCharacter>(OtherActor))
		{
			Super::OnOverlapBegin(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

			bIsPickuped = true;

			// Subsystemを取得
			UGameInstance* GameInstance = GetGameInstance();
			if (GameInstance)
			{
				UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
				if (ProgressionSubsystem)
				{
					// ステージ番号を伝えて、そのステージの獲得メダル数に+1する
					ProgressionSubsystem->AddMedalCount(mStageIndex);

					// 自分のメダルIDを伝えて仮の辞書に「獲得済み」として登録する（一時保存）
					ProgressionSubsystem->SetMedalCollectedTemp(mMedalID);
				}
			}
			// Destroy();
		}
	}
}