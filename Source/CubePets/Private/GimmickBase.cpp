// Fill out your copyright notice in the Description page of Project Settings.


#include "GimmickBase.h"
#include "AbilitySystemComponent.h"

// Sets default values
AGimmickBase::AGimmickBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	mAbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	mAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

UAbilitySystemComponent* AGimmickBase::GetAbilitySystemComponent() const
{
	return mAbilitySystemComponent;
}

// Called when the game starts or when spawned
void AGimmickBase::BeginPlay()
{
	Super::BeginPlay();
	
	// ASC‚Ì‰Šú‰»
	if (mAbilitySystemComponent)
	{
		mAbilitySystemComponent->InitAbilityActorInfo(this, this);

		if (HasAuthority())
		{
			for (TSubclassOf<UGameplayAbility> StartupAbility : mDefaultAbilities)
			{
				if (StartupAbility)
				{
					mAbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(StartupAbility, 1, 0));
				}
			}
		}
	}
}

// Called every frame
void AGimmickBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

