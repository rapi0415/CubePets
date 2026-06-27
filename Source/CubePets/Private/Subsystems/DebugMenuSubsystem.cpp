// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/DebugMenuSubsystem.h"
#include "Characters/CubePetsCharacter.h"
#include "Gimmicks/CheckPointActor.h"
#include "Kismet/GameplayStatics.h"
#include "Subsystems/GameProgressionSubsystem.h"

#if !UE_BUILD_SHIPPING
#include <imgui.h>
#include "ImGuiModule.h"
#endif

void UDebugMenuSubsystem::Tick(float DeltaTime)
{

#if !UE_BUILD_SHIPPING
	
	if (!GetWorld() || !GetWorld()->IsGameWorld()) return;

	FImGuiModule& ImGuiModule = FImGuiModule::Get();
	bool bIsImGuiActive = ImGuiModule.GetProperties().IsInputEnabled();

	// デバッグ表示ONじゃないなら表示しないので終了
	if (!bIsImGuiActive) return;

	// ImGuiの開始
	ImGui::Begin("Debug Menu");

	if (ImGui::BeginTabBar("DebugMenuTabBar"))
	{
		if (ImGui::BeginTabItem("Player"))
		{
			RenderPlayerMenu();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Stage"))
		{
			RenderStageMenu();
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("System"))
		{
			RenderSystemMenu();
			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}

	ImGui::End();

#endif
}

TStatId UDebugMenuSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDebugMenuSubsystem, STATGROUP_Tickables);
}

bool UDebugMenuSubsystem::IsTickable() const
{
#if !UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}

bool UDebugMenuSubsystem::ShouldCreateSubsystem(UObject* Outer) const
{
	if (!Super::ShouldCreateSubsystem(Outer))
	{
		return false;
	}

#if !UE_BUILD_SHIPPING
	return true;
#else
	return false;
#endif
}

#if !UE_BUILD_SHIPPING
void UDebugMenuSubsystem::RenderPlayerMenu()
{
	ACubePetsCharacter* Player = Cast<ACubePetsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Player)
	{
		ImGui::Text("Player not found!");
		return;
	}

	if (ImGui::CollapsingHeader("Cheats", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// プレイヤーを無敵にする機能
		ImGui::Checkbox("Invincible Mode", &Player->bIsInvincible);
	}

	if (ImGui::CollapsingHeader("Status"))
	{
		// プレイヤーの座標を表示する機能
		FVector Pos = Player->GetActorLocation();
		ImGui::Text("Location: X:%.1f, Y:%.1f, Z:%.1f", Pos.X, Pos.Y, Pos.Z);
	}
}

void UDebugMenuSubsystem::WarpToLocation(FVector TargetLocation)
{
	ACubePetsCharacter* Player = Cast<ACubePetsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (Player)
	{
		Player->TeleportTo(TargetLocation, Player->GetActorRotation());
	}
}

void UDebugMenuSubsystem::RenderStageMenu()
{
	// プレイヤーをワープさせる機能
	if (ImGui::CollapsingHeader("Warp Points", ImGuiTreeNodeFlags_DefaultOpen))
	{
		// ステージ内の全チェックポイントを検索して全部ボタンにしていく
		TArray<AActor*> CheckPoints;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACheckPointActor::StaticClass(), CheckPoints);

		if (CheckPoints.IsEmpty())
		{
			ImGui::Text("No CheckPoints found in this level!");
		}
		else
		{
			for (int32 i = 0; i < CheckPoints.Num(); ++i)
			{
				AActor* PointActor = CheckPoints[i];

				FString ButtonName = FString::Printf(TEXT("Warp to %s"), *PointActor->GetName());

				if (ImGui::Button(TCHAR_TO_UTF8(*ButtonName)))
				{
					WarpToLocation(PointActor->GetActorLocation());
				}
			}
		}
	}

	// 全ステージ解放する機能
	if (ImGui::CollapsingHeader("Unlock Stage", ImGuiTreeNodeFlags_DefaultOpen))
	{
		UGameInstance* GameInstance = GetGameInstance();
		if (!GameInstance)
		{
			ImGui::Text("GameInstance not found!");
			return;
		}
		UGameProgressionSubsystem* PSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (!PSubsystem)
		{
			ImGui::Text("Subsystem not found!");
			return;
		}

		if (ImGui::Button("Unlock All Stages"))
		{
			PSubsystem->UnlockAllStages();
		}
	}
}

void UDebugMenuSubsystem::RenderSystemMenu()
{
	ACubePetsCharacter* Player = Cast<ACubePetsCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

	if (!Player)
	{
		ImGui::Text("Player not found!");
		return;
	}

	if (ImGui::Button("Deactivated All Cubes"))
	{
		Player->DeactivateAllCubes();
	}
}
#endif // !UE_BUILD_SHIPPING

