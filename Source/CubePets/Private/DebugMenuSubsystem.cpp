// Fill out your copyright notice in the Description page of Project Settings.


#include "DebugMenuSubsystem.h"
#include "CubePetsCharacter.h"
#include "Kismet/GameplayStatics.h"
#include <imgui.h>
#include "ImGuiModule.h"

void UDebugMenuSubsystem::Tick(float DeltaTime)
{
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

		ImGui::EndTabBar();
	}

	ImGui::End();
}

TStatId UDebugMenuSubsystem::GetStatId() const
{
	RETURN_QUICK_DECLARE_CYCLE_STAT(UDebugMenuSubsystem, STATGROUP_Tickables);
}

bool UDebugMenuSubsystem::IsTickable() const
{
	return true;
}

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
