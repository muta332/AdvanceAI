// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceAI/Public/Systems/GunSystem/GGunSystem.h"

#include <rapidjson/reader.h>

#include "AdvanceAI/Public/InterFaces/CharacterInterface.h"
#include "AdvanceAI/Public/BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/Systems/AttackSystem/AttackSystem.h"
#include "AdvanceAI/Macros.h"

#include "Fonts/UnicodeBlockRange.h"
#include "Player/ABasePlayerCharacter.h"


// Sets default values for this component's properties
UGGunSystem::UGGunSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	FireRate = 0.1;

	Ammo = 25;

	MaxAmmo = 25;

	AmmoStorage = 100;

	BrustCount = 3;

	bIsAI = false;

	TraceLength = 90000;

	MaxAmmoStorage = AmmoStorage;

	LoopCounterForRiflingMode = -1;

	MaxLoopCounterForRiflingMode = 2;
}

void UGGunSystem::Triggered()
{
	OnFireStarted.Broadcast();

	if (GetOwner()->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(GetOwner());
		

		if (TagContainer.HasTag(CameraZoomTag))
		{
			GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this,&UGGunSystem::Attack, FireRate, true );
		}
		
	}
}

void UGGunSystem::TriggerReleased()
{
	ResetAttack();

	AttackTimerHandle.Invalidate();

	OnFireEnded.Broadcast();

	if (GetOwner()->Implements<UCharacterInterface>())
	{
		ICharacterInterface::Execute_MontageStop(GetOwner(), AttackMontage.MontageToPlay);
	}
	if ((Ammo < 0 && Ammo == 0) && !(AmmoStorage < 0 && AmmoStorage == 0) && GetOwner()->Implements<UCharacterInterface>())
	{
		ICharacterInterface::Execute_PlayAnimation(GetOwner(), ReloadMontage );
	}
	
}

void UGGunSystem::SwitchFiringMode()
{
	if (GetOwner()->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(GetOwner());
		
		if (!TagContainer.HasTag(ShootingStatus))
		{
			UGunBlueprintFunctionLibrary::DeleteTagBatch(GetOwner(), FiringModeTag);

			SwitchMode();
		}
	}
}

void UGGunSystem::Reloading()
{
	if (!Ammo == MaxAmmo && !(AmmoStorage < 0 && AmmoStorage == 0))
	{
		if (GetOwner()->Implements<UCharacterInterface>())
		{
			ICharacterInterface::Execute_PlayAnimation(GetOwner(), ReloadMontage);
		}
	}
}


// Called when the game starts
void UGGunSystem::BeginPlay()
{
	Super::BeginPlay();

	AsCharacter = Cast<ABasePlayerCharacter>(GetOwner());

	AttackSystem = GetOwner()->FindComponentByClass<UAttackSystem>();
	
	
}

void UGGunSystem::ResetAttack()
{
}

void UGGunSystem::SwitchOnFiringMode(const FGameplayTag& Tag)
{
/*	switch(Tag)
	{
	case FiringModeAuto:

		break;
	case FiringModeBrust:

		break;

	case FiringModeSingle:
		break;
	}
	*/
}

void UGGunSystem::Attack()
{
	if (Ammo > 0 && GetOwner()->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(GetOwner());

		for (const FGameplayTag& Tag : TagContainer.GetGameplayTagArray())
		{
			if (Tag.MatchesTag(FiringModeTag))
			{
                 SwitchOnFiringMode(Tag);
			}
		}
	}
}

void UGGunSystem::AttackAmmoCalculation()
{
}

FProjectileInfo UGGunSystem::CameraAim()
{
	return FProjectileInfo();
}

void UGGunSystem::AttackAssistantFunction()
{
}

void UGGunSystem::AmmoUpdate()
{
}

void UGGunSystem::SwitchMode()
{
}

void UGGunSystem::Shoot()
{
	AttackAssistantFunction();
	
	if (GetOwner()->Implements<UGamePlayTagInterFaces>() && GetOwner()->Implements<UCharacterInterface>())
	{
		IGamePlayTagInterFaces::Execute_AddTag(GetOwner(), ShootingStatus);

		ICharacterInterface::Execute_PlayAnimation(GetOwner(), AttackMontage);
	}
}

void UGGunSystem::ModeAutoFunction()
{
	Shoot();
}

void UGGunSystem::ModeBurstFunction()
{
	

//DoN(BrustCount)
	

	
}

void UGGunSystem::ModeSingleFunction()
{
}

void UGGunSystem::DoN(int32 N)
{
	static int32 Counter = 0;
	if (!(Counter > N))
	{
		Shoot();
		Counter++;
	}
	else
	{
		AttackTimerHandle.Invalidate();
	}
}



