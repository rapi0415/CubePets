// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MedalItemBase.h"
#include "Characters/CubePetsCharacter.h"
#include "Subsystems/GameProgressionSubsystem.h"

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
				}
			}
		}
	}
}
