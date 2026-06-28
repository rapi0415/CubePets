// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/CubePetsCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Objects/CubeReticle.h"
#include "Objects/CubePetsCube.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "System/InGame/CubePetsPlayerController.h"
#include "System/InGame/CubePetsGameModeBase.h"
#include "Subsystems/GameProgressionSubsystem.h"
#include "Subsystems/CheckPointSubsystem.h"

// Sets default values
ACubePetsCharacter::ACubePetsCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// カメラ設定
	mCameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	mCameraBoom->SetupAttachment(RootComponent);
	mCameraBoom->bUsePawnControlRotation = true;

	mFollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	mFollowCamera->SetupAttachment(mCameraBoom);
}

void ACubePetsCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// EnhancedInput用の処理（入力マッピングの登録）
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(mDefaultMappingContext, 0);
		}
	}

	UWorld* World = GetWorld();

	// 箱のスポーン用処理：オブジェクトプールの構築（あらかじめ使う数だけスポーンして非アクティブにしておく）
	if (mCubePetsCubeClass != nullptr && World != nullptr)
	{
		mCubePetsCubeArray.Reserve(mMaxCube);
		
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;

		for (int32 i = 0; i < mMaxCube; ++i)
		{
			ACubePetsCube* NewCube = World->SpawnActor<ACubePetsCube>(mCubePetsCubeClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

			if (NewCube)
			{
				NewCube->OnDeactivated();

				mCubePetsCubeArray.Add(NewCube);
			}
		}
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
		if (ProgressionSubsystem)
		{
			// Subsystemの持つ「箱を使った数」をチェックポイント通過時に保存してある数に戻す（最初は0）
			ProgressionSubsystem->RestoreToSavedCubeCount();

			// 一時保存用の辞書にしか登録していないメダルをリセット
			ProgressionSubsystem->ResetMedalCollectedTemp();

		}

		// チェックポイント記録を見てワープする
		UCheckPointSubsystem* CPSubsystem = GameInstance->GetSubsystem<UCheckPointSubsystem>();
		if (CPSubsystem && CPSubsystem->HasCheckPoint())
		{
			SetActorLocation(CPSubsystem->GetCurrentCheckPoint(), false, nullptr, ETeleportType::TeleportPhysics);
		}
	}
}

void ACubePetsCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 足場が変わった時だけ発光を更新する
	AActor* CurrentBase = GetMovementBaseActor(this);
	if (CurrentBase != mLastPlayerBase)
	{
		mLastPlayerBase = CurrentBase;
		UpdateOldestCubeGlow();
	}
}

// Called to bind functionality to input
void ACubePetsCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(mMoveAction, ETriggerEvent::Triggered, this, &ACubePetsCharacter::Move);
		EnhancedInputComponent->BindAction(mLookAction, ETriggerEvent::Triggered, this, &ACubePetsCharacter::Look);
		EnhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Started, this, &ACubePetsCharacter::JumpAction);
		EnhancedInputComponent->BindAction(mJumpAction, ETriggerEvent::Completed, this, &ACubePetsCharacter::StopJumpAction);
		EnhancedInputComponent->BindAction(mCreateAction, ETriggerEvent::Triggered, this, &ACubePetsCharacter::CreateAction);
		EnhancedInputComponent->BindAction(mLockRotationAction, ETriggerEvent::Triggered, this, &ACubePetsCharacter::LockRotation);
		EnhancedInputComponent->BindAction(mLockRotationAction, ETriggerEvent::Completed, this, &ACubePetsCharacter::UnlockRotation);
	}
}

// 次に消える箱を光らせる関数
void ACubePetsCharacter::UpdateOldestCubeGlow()
{
	bool bIsMaxSpawned = true;

	// 箱を全部出し切っていないなら終了
	for (ACubePetsCube* Cube : mCubePetsCubeArray)
	{
		if (Cube && !Cube->GetIsActive())
		{
			bIsMaxSpawned = false;
			break;
		}
	}

	// いったん全部光らせないようにする
	for (ACubePetsCube* Cube : mCubePetsCubeArray)
	{
		if (Cube)
		{
			Cube->SetGlow(false);
		}
	}

	// 出し切っているなら次の候補の箱を光らせる
	if (bIsMaxSpawned)
	{
		// プレイヤーが乗っている箱はスキップして光らせる
		AActor* PlayerBase = GetMovementBaseActor(this);
		for (int32 i = 0; i < mMaxCube; ++i)
		{
			int32 CheckIndex = (mNextIndex + i) % mMaxCube;
			ACubePetsCube* CandidateCube = mCubePetsCubeArray[CheckIndex];

			if (CandidateCube && CandidateCube != PlayerBase)
			{
				CandidateCube->SetGlow(true);
				break;
			}
		}
	}
}

// ダメージを受けたときの処理
float ACubePetsCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{

#if !UE_BUILD_SHIPPING
	// 無敵状態ならダメージ0で終了（デバッグ用）
	if (bIsInvincible)
	{
		return 0.0f;
	}
#endif

	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.0f && !bIsDead)
	{
		bIsDead = true;

		// 入力を無効にする
		if (APlayerController* PC = Cast<APlayerController>(GetController()))
		{
			DisableInput(PC);
			PC->DisableInput(PC);
		}

		// ラグドールにする
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);

		// BGMを止める
		if (UWorld* World = GetWorld())
		{
			if (ACubePetsGameModeBase* GM = Cast<ACubePetsGameModeBase>(World->GetAuthGameMode()))
			{
				GM->StopStageBGM();
			}
		}

		// 一定時間後にフェードアウト演出開始させる
		GetWorldTimerManager().SetTimer(mRestartTimerHandle, this, &ACubePetsCharacter::BroadcastFadeOut, mRestartDelay, false);
	}

	return ActualDamage;
}

// フェードアウト用のデリゲート、登録されている関数を呼び出す
void ACubePetsCharacter::BroadcastFadeOut()
{
	mOnFadeOutTriggered.Broadcast();
}

// 生成時のエフェクト生成用関数（共通処理を後で入れることを想定してネイティブ関数でラップする）
void ACubePetsCharacter::PlayCreateEffects()
{
	BP_PlayCreateEffects();
}

void ACubePetsCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);

	}
}

void ACubePetsCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACubePetsCharacter::JumpAction(const FInputActionValue& Value)
{
	// ボタンが一度離されている場合のみジャンプする
	/*
	if (bIsJumpButtonReleased)
	{
		Jump();
		bIsJumpButtonReleased = false;
	}
	*/
	Jump();
}

void ACubePetsCharacter::StopJumpAction(const FInputActionValue& Value)
{
	StopJumping();
	// bIsJumpButtonReleased = true;
}

void ACubePetsCharacter::CreateAction(const FInputActionValue& Value)
{
	if (!mReticleClass || !mCubePetsCubeClass) return;

	// --状態1：箱の生成の準備-- ブロックレティクル生成中でなければ、まずレティクルを生成
	if(!bIsReticleExistence)
	{
		UWorld* World = GetWorld();

		if(World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();

			FVector SpawnLocation = GetActorLocation() + (GetActorForwardVector() * mSpawnOffset);

			// スポーン
			mCubeReticle = World->SpawnActor<ACubeReticle>(mReticleClass, SpawnLocation, GetActorRotation(), SpawnParams);

			// プレイヤーにアタッチする
			if (mCubeReticle)
			{
				FAttachmentTransformRules AttachRules(EAttachmentRule::KeepWorld, true);
				mCubeReticle->AttachToComponent(GetMesh(), AttachRules);
			}
			bIsReticleExistence = true;

			// PlayerControllerに通知（操作ガイド用テキスト更新のため）
			ACubePetsPlayerController* PC = Cast<ACubePetsPlayerController>(GetController());
			if (PC)
			{
				PC->NotifyReticleStateChanged(bIsReticleExistence);
			}

		}
	}
	// --状態2：実際の箱の生成-- ブロックレティクル生成中にボタンを押した→生成許可状態ならブロック生成
	else
	{
		if (mCubeReticle && mCubeReticle->GetIsPermission())
		{
			FVector SpawnLocation = mCubeReticle->GetActorLocation();
			FRotator SpawnRotation = mCubeReticle->GetActorRotation();


			AActor* PlayerBase = GetMovementBaseActor(this); // プレイヤーが乗っているアクターを取得

			// プレイヤーが乗っているブロックは候補から外して、ブロックを再生成する
			for (int32 i = 0; i < mMaxCube; ++i)
			{
				ACubePetsCube* TargetCube = mCubePetsCubeArray[mNextIndex]; // 候補を決める

				if (!TargetCube || TargetCube == PlayerBase)
				{
					mNextIndex = (mNextIndex + 1) % mMaxCube; // プレイヤーが乗っていたら次の候補に移る（候補がnullptrだったときも）
					continue;
				}

				TargetCube->OnDeactivated();
				TargetCube->OnActivated(SpawnLocation, SpawnRotation);

				// mNextIndexを1個繰り上げ
				mNextIndex = (mNextIndex + 1) % mMaxCube;

				// 次消える候補の箱があれば光らせる
				UpdateOldestCubeGlow();

				// Subsystemの持つ「箱を使った数」を更新
				UGameInstance* GameInstance = GetGameInstance();
				if (GameInstance)
				{
					UGameProgressionSubsystem* ProgressionSubsystem = GameInstance->GetSubsystem<UGameProgressionSubsystem>();
					if (ProgressionSubsystem)
					{
						ProgressionSubsystem->AddUsedCubeCount();
					}
				}

				break;
			}

			// 効果音を鳴らす
			PlayCreateEffects();
			
			// レティクルは消す
			mCubeReticle->Destroy();
			mCubeReticle = nullptr;

			bIsReticleExistence = false;

			// PlayerControllerに通知（操作ガイド用テキストを更新するため）
			ACubePetsPlayerController* PC = Cast<ACubePetsPlayerController>(GetController());
			if (PC)
			{
				PC->NotifyReticleStateChanged(bIsReticleExistence);
			}
		}	
	}
}

void ACubePetsCharacter::LockRotation(const FInputActionValue& Value)
{
	// 向きを固定する
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->bOrientRotationToMovement = false;
	}
}

void ACubePetsCharacter::UnlockRotation(const FInputActionValue& Value)
{
	// ボタンを離したら向き固定解除
	UCharacterMovementComponent* MoveComp = GetCharacterMovement();
	if (MoveComp)
	{
		MoveComp->bOrientRotationToMovement = true;
	}
}

#if !UE_BUILD_SHIPPING
// すべての箱を消す関数（デバッグ用）
void ACubePetsCharacter::DeactivateAllCubes()
{
	for (ACubePetsCube* Cube : mCubePetsCubeArray)
	{
		if (IsValid(Cube))
		{
			Cube->OnDeactivated();
		}
	}
}
#endif

