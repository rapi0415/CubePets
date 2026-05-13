// Fill out your copyright notice in the Description page of Project Settings.


#include "CubeReticle.h"
#include "Components/BoxComponent.h"

// Sets default values
ACubeReticle::ACubeReticle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// StaticMesh
	mStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	RootComponent = mStaticMesh;
	mStaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	mStaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	// BoxTrigger
	mBoxTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTrigger"));
	mBoxTrigger->SetupAttachment(mStaticMesh);
	mBoxTrigger->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	mBoxTrigger->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void ACubeReticle::BeginPlay()
{
	Super::BeginPlay();

	// 動的マテリアルを作成
	if (mStaticMesh)
	{
		mDynamicMaterial = mStaticMesh->CreateDynamicMaterialInstance(0);
	}
}

// Called every frame
void ACubeReticle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// 以下、壁に触れているかどうかの判定
	if (!mBoxTrigger || !mDynamicMaterial) return;

	// 判定に使うboxの形状を作成 (BoxTriggerのスケールに合わせる)
	FCollisionShape BoxShape = FCollisionShape::MakeBox(mBoxTrigger->GetScaledBoxExtent());

	// 判定のクエリパラメータ設定
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // 自分自身は無視

	// Overlapテストを実行
	bool bIsOverlapping = GetWorld()->OverlapAnyTestByChannel
	(
		mBoxTrigger->GetComponentLocation(), // 位置
		mBoxTrigger->GetComponentQuat(), // 回転
		ECC_Visibility, // チャンネル
		BoxShape, // 形状
		Params
	);

	// 結果に応じて色を変更
	if (bIsOverlapping)
	{
		mDynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), FLinearColor::Red);
		bIsPermission = false;
	}
	else
	{
		FLinearColor CubeColor = FLinearColor(0.2f, 0.9f, 1.0f, 1.0f);
		mDynamicMaterial->SetVectorParameterValue(TEXT("BaseColor"), CubeColor);
		bIsPermission = true;
	}
	// ここまで（壁に触れているかどうかの判定）

}



