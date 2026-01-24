// Fill out your copyright notice in the Description page of Project Settings.


#include "Systems/HealhtAndDamageSystem/GHealthAndDamageSystem.h"
#include "AdvanceAI/Public/InterFaces/GCharacterInterface.h"

#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"

// Sets default values for this component's properties
UGHealthAndDamageSystem::UGHealthAndDamageSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


float UGHealthAndDamageSystem::Heal_Implementation(const float& HealAmount)
{
    Health = FMath::Clamp(Health + HealAmount, 0, MaxHealth);
	OnHealed.Broadcast(Health, MaxHealth);
	return Health;
}

// Called when the game starts
void UGHealthAndDamageSystem::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UGHealthAndDamageSystem::DoDamage(const FDamageInfo& DamageInfo)
{
	Health = FMath::Clamp(Health- DamageInfo.Amount, 0, MaxHealth);

	OnDamageResponse.Broadcast(DamageInfo.DamageResponse, DamageInfo.DamageCauser,DamageInfo.DamageType );

	if (Health < 0 || Health == 0)
	{
		IsDead = true;

		OnDeath.Broadcast();

		IGamePlayTagInterFaces::Execute_AddTag(this->GetOwner(),FGameplayTag::RequestGameplayTag("Status.Dead"));
		
	}
	else if (DamageInfo.ShouldForceInterrupt ||  IsInterruptable)
	{
		IGCharacterInterface::Execute_MontageStop(this->GetOwner(),nullptr);
	}
	else
	{
		
	}
}

void UGHealthAndDamageSystem::NoDamage(bool WasParrySuccessful, const FDamageInfo& DamageInfo)
{
	
	if (WasParrySuccessful)
	{
		OnParried.Broadcast(DamageInfo.DamageCauser);
	}
	else if (!WasParrySuccessful)
	{
		OnBlocked.Broadcast(DamageInfo.DamageCauser);
	}
}

void UGHealthAndDamageSystem::TakeDamage_Implementation(const FDamageInfo& DamageInfo, bool& WasDamaged)
{
	DamageCauser = DamageInfo.DamageCauser;
	if (DamageInfo.CanBeBlocked && IsBlocking)
	{
		//NoDamage path
		if (DamageInfo.CanBeParried && IsParrying)
		{
			NoDamage(true, DamageInfo);
		}
		else
		{
			NoDamage(false,DamageInfo);
		}
		WasDamaged = false;
		
	}
	else
	{
		//Damage Path
		DoDamage(DamageInfo);
		WasDamaged = true;
		if (Health < ExecuteOnLowHealthfloat || Health == ExecuteOnLowHealthfloat)
		{
			OnLowOnHealth.Broadcast(DamageInfo.DamageCauser, Health);
		}
			
			
	}
}



// Called every frame
void UGHealthAndDamageSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

