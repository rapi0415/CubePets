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

	// PlayerControllerを取得（メダル獲得状況をリセットする関数をバインド）
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		ACubePetsPlayerController* CubePetsPC = Cast<ACubePetsPlayerController>(PC);
		if (CubePetsPC)
		{
			CubePetsPC->mOnResetStageInfo.AddUObject(this, &AMedalItemBase::ResetMedal);
		}
	}
}

void AMedalItemBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC)
	{
		ACubePetsPlayerController* CubePetsPC = Cast<ACubePetsPlayerController>(PC);
		if (CubePetsPC)
		{
			CubePetsPC->mOnResetStageInfo.RemoveAll(this);
		}
	}
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

void AMedalItemBase::ResetMedal()
{
	// Subsystemを取得
	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			// 獲得済みならメダルの数を-1する（獲得したら+1されてるはずなのでこれでリセットできる）
			/*
			if (bIsPickuped)
			{
				// ProgressionSubsystem->SubtractMedalCount(mStageIndex);
			}
			*/

			// 獲得が確定してるならリセットしなくて良いので終了する
			if (ProgressionSubsystem->IsMedalAlreadyCollected(mMedalID))
			{
				return;
			}

			// 獲得状況管理用Mapから除外する（）
			ProgressionSubsystem->ResetMedalCollected(mMedalID);
		}
	}
}
