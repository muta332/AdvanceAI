// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceAI/Public/Player/GCharacter.h"

#include "AudioDevice.h"
#include "AdvanceAI/Macros.h"
#include "AdvanceAI/Public/Weapon/Gun/GBaseGun.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Math/TransformCalculus3D.h"
#include "Systems/GunSystem/GGunSystem.h"
#include "AdvanceAI/Public/Systems/HealhtAndDamageSystem/GHealthAndDamageSystem.h"
#include "Chaos/HierarchicalSpatialHash.h"
#include "Components/CapsuleComponent.h"


// Sets default values
AGCharacter::AGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MaxRunSpeed = 500;

	CrouchAndWalkSpeed = 300;

	MaxSprintSpeed = 800;

	HealthAndDamageSystem = CreateDefaultSubobject<UGHealthAndDamageSystem>(TEXT("Health And Damage System"));
	
	

	

}




void AGCharacter::OnStanceChange_Implementation(FGameplayTag StanceTag)
{
	UGunBlueprintFunctionLibrary::DeleteTagBatch(this, FGameplayTag::RequestGameplayTag("Stance"));
	if (StanceTag == FGameplayTag::RequestGameplayTag("Stance.Crouching"))
	{
		SwitchOnStance(StanceTag, CrouchAndWalkSpeed);
	}
	else if(StanceTag == FGameplayTag::RequestGameplayTag("Stance.Running"))
	{
		SwitchOnStance(StanceTag, MaxRunSpeed);
	}
	else if (StanceTag == FGameplayTag::RequestGameplayTag("Stance.Sprinting"))
	{
		SwitchOnStance(StanceTag, MaxSprintSpeed);
	}
	else if (StanceTag == FGameplayTag::RequestGameplayTag("Stance.Walking"))
	{
		SwitchOnStance(StanceTag, CrouchAndWalkSpeed);
	}
	
}

void AGCharacter::StanceChangeSprint_Implementation()
{
	if ( !ActiveGameplayTagContainer.HasTag(FGameplayTag::RequestGameplayTag("Stance.Crouching")))
	{
		OnStanceChange(FGameplayTag::RequestGameplayTag(FName("Stance.Sprinting")));
	}

}

void AGCharacter::StanceChangeRunning_Implementation()
{
	if (!ActiveGameplayTagContainer.HasTag(FGameplayTag::RequestGameplayTag("Stance.Crouching")))
	{
		OnStanceChange(FGameplayTag::RequestGameplayTag(FName("Stance.Running")));
	}

}

void AGCharacter::StanceChangeWalking_Implementation()
{
	if (!bWalkingFlipFlop)
	{
		OnStanceChange(FGameplayTag::RequestGameplayTag(FName("Stance.Walking")));
		bWalkingFlipFlop = true;
	}
	else
	{
		OnStanceChange(FGameplayTag::RequestGameplayTag(FName("Stance.Running")));
		bWalkingFlipFlop = false;
	}
}

void AGCharacter::StanceChangeCrouching_Implementation()
{
	if (!bCrouchingFlipFlop)
	{
		OnStanceChange(FGameplayTag::RequestGameplayTag(FName("Stance.Crouching")));
		StanceMode = EStanceMode::ESM_Crouching;
		UpdateCameraBoomForCrouchFunction();
		bCrouchingFlipFlop = true;
	}
	else
	{
		OnStanceChange(FGameplayTag::RequestGameplayTag(FName("Stance.Running")));
		StanceMode = EStanceMode::ESM_Running;
		UpdateCameraBoomForCrouchReverseFunction();
		bCrouchingFlipFlop = false;
	}
}

void AGCharacter::OnDamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageCauser, EDamageType DamageType)
{
	PlayDamageResponseAnim(DamageType);
}

void AGCharacter::OnDeath_Implementation()
{
	FMontage* Montage = nullptr;
	DisableInput(Cast<APlayerController>(this->GetController()));
	
	Montage = AnimMontageContainers.Find(DeathTag);
	if (Montage)
	{
		IGCharacterInterface::Execute_PlayAnimation(this, *Montage);

		SetLifeSpan(LifeSpanAfterdeath);

		GetCapsuleComponent()->DestroyComponent();
	}
}

void AGCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (!ensure((GetMesh()))) return;
	AnimInstance = GetMesh()->GetAnimInstance();
	CreateMainWidget();
	FunctionBinding();
	DamageSystemBinding();
	
	
}

void AGCharacter::AddTag_Implementation(const FGameplayTag AddTag)
{
	IGamePlayTagInterFaces::AddTag_Implementation(AddTag);
	ActiveGameplayTagContainer.AddTag(AddTag);
}

void AGCharacter::RemoveTag_Implementation(const FGameplayTag RemoveTag)
{
	IGamePlayTagInterFaces::RemoveTag_Implementation(RemoveTag);
	ActiveGameplayTagContainer.RemoveTag(RemoveTag);
}

void AGCharacter::PlayAnimation_Implementation(const FMontage& AnimMontage)
{
	IGCharacterInterface::PlayAnimation_Implementation(AnimMontage);
	if (!ActiveGameplayTagContainer.HasTagExact(FGameplayTag::RequestGameplayTag(FName("AnimGaurd.AnimPlaying"))) && AnimInstance && !ActiveGameplayTagContainer.HasTag(FGameplayTag::RequestGameplayTag("Status.Dead")))
	{
		ActiveGameplayTagContainer.AddTag(FGameplayTag::RequestGameplayTag(FName("AnimGaurd.AnimPlaying")));
		AnimInstance->Montage_Play(AnimMontage.MontageToPlay, AnimMontage.InPlayRate, EMontagePlayReturnType::MontageLength, 0, AnimMontage.bStopMontages);
		AnimInstance->Montage_JumpToSection(AnimMontage.SectionName,AnimMontage.MontageToPlay);
	}
}

void AGCharacter::MontageStop_Implementation(UAnimMontage* AnimMontage)
{
	IGCharacterInterface::MontageStop_Implementation(AnimMontage);
	if (AnimInstance)
	{
		AnimInstance->Montage_Stop(0.2, AnimMontage);
	}
	
}

void AGCharacter::OnUpdateAmmoEvent_Implementation(const int32& Ammo)
{
}

void AGCharacter::UpdateCameraBoomForCrouchFunction_Implementation()
{
}

void AGCharacter::UpdateCameraBoomForCrouchReverseFunction_Implementation()
{
}

void AGCharacter::SetSpeed_Implementation(float Speed)
{
	IGCharacterInterface::SetSpeed_Implementation(Speed);
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

FGameplayTagContainer AGCharacter::GetActiveGamePlayTagContainer_Implementation()
{
	return ActiveGameplayTagContainer;
}

void AGCharacter::EquipWeapon_Implementation(TSubclassOf<AGBaseGun> WeaponClass)
{
	IGCharacterInterface::EquipWeapon_Implementation(WeaponClass);

	SpawnPrimaryWeapon(WeaponClass);
}

AGCharacter* AGCharacter::GetCharacter_Implementation()
{
	return this;
}

void AGCharacter::StorageInteraction_Implementation(const int32& UpdateAmmo)
{
	IGCharacterInterface::StorageInteraction_Implementation(UpdateAmmo);
}

void AGCharacter::AnimationBinding_Implementation()
{
	if (!ensure(AnimInstance)) return;
	AnimInstance->OnMontageEnded.AddDynamic(this, &AGCharacter::OnMontageEnded);
}

void AGCharacter::InteractionSystemBinding_Implementation()
{
}

void AGCharacter::GunSystemBinding_Implementation()
{
	if (!Weapon)return;
	UGGunSystem* System = Weapon->GunSystem;
	if (System)
	{
		System->OnFireEnded.AddDynamic(this, &AGCharacter::OnFireEnded_Event);
		System->OnFireStarted.AddDynamic(this, &AGCharacter::OnFireStarted_Event);
		System->UpdateCurrentStorage.AddDynamic(this, &AGCharacter::UpdateStorage_Event);
		System->OnUpdateMag.AddDynamic(this, &AGCharacter::OnUpdateAmmoEvent);
		System->OnReloadFinished.AddDynamic(this, &AGCharacter::OnReloadFinished_Event);
	}
}



void AGCharacter::UpdateStorage_Event_Implementation(int32 CurrentStorage)
{
}

void AGCharacter::NoInteraction_Implementation()
{
}

void AGCharacter::WidgetInteract_Implementation(AActor* InteractActor)
{
}

void AGCharacter::OnReloadFinished_Event_Implementation(const int32& Ammo, const int32& StorageAmmo)
{
}

void AGCharacter::OnFireEnded_Event_Implementation()
{
}

void AGCharacter::OnFireStarted_Event_Implementation()
{
}

void AGCharacter::SpawnWeaponLogic(TSubclassOf<AGBaseGun> WeaponClass)
{
	FTransform EmtyTransform;
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
		
	Weapon = GetWorld()->SpawnActorDeferred<AGBaseGun>(WeaponClass,EmtyTransform,this, this);
	if (Weapon)
	{
		Weapon->bIsAttachedToCharacter = true;
		IGamePlayTagInterFaces::Execute_AddTag(Weapon, FGameplayTag::RequestGameplayTag("Status.NotInteractable"));
		IGamePlayTagInterFaces::Execute_AddTag(Weapon, FGameplayTag::RequestGameplayTag("Gun.Claimed"));
		
		Weapon->FinishSpawning(EmtyTransform);
		IGamePlayTagInterFaces::Execute_AddTag(this, FGameplayTag::RequestGameplayTag("Status.CarryingGun"));
		
	}
}



void AGCharacter::SwitchOnStance(const FGameplayTag& SwitchTag, float Speed)
{
	ActiveGameplayTagContainer.AddTag(SwitchTag);
	IGCharacterInterface::Execute_SetSpeed(this, Speed);
}

int32 AGCharacter::GetTeamNumber_Implementation()
{
	return TeamNumber;
}

int32 AGCharacter::GetTeamNumberWithInterface(AActor* OtherActor)
{
	if (OtherActor->Implements<UGCharacterInterface>())
	{
		return IGCharacterInterface::Execute_GetTeamNumber(OtherActor);
	}
	return 0;
}

bool AGCharacter::CheckifHostile_Implementation(AActor* OtherActor)
{
	if (TeamNumber == GetTeamNumberWithInterface(OtherActor))
	{
		return false;
	}
	return true;
}

void AGCharacter::GetWeapon_Implementation(AGBaseGun*& AsWeapon)
{
	if (!Weapon) return;
	AsWeapon = Weapon;
}

void AGCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	
	
}

void AGCharacter::DamageSystemBinding()
{
	if (HealthAndDamageSystem)
	{
		HealthAndDamageSystem->OnDamageResponse.AddDynamic(this, &AGCharacter::OnDamageResponse);
		HealthAndDamageSystem->OnDeath.AddDynamic(this, &AGCharacter::OnDeath);
	}
}

bool AGCharacter::TakeDamage_Implementation(const FDamageInfo& DamageInfo)
{
	bool WasDamaged;
	HealthAndDamageSystem->TakeDamage(DamageInfo, WasDamaged);
	return WasDamaged;
}

void AGCharacter::PlayDamageResponseAnim(EDamageType DamageType)
{
	if (HealthAndDamageSystem && !(HealthAndDamageSystem->Health < 0 || HealthAndDamageSystem->Health ==0))
	{
		FMontage* Montage = nullptr;
		if (DamageType == EDamageType::EDT_Projectile)
		{
			Montage = AnimMontageContainers.Find(ProjectileReactionTag);
		}
		else if ( DamageType== EDamageType::EDT_Explosion )
		{
			Montage = AnimMontageContainers.Find(ExplosionReactionTag);
		}
		else if (DamageType == EDamageType::EDT_Melee)
		{
			Montage = AnimMontageContainers.Find(MeleeReactionTag);
		}
		if (!Montage) return;
		IGCharacterInterface::Execute_PlayAnimation(this, *Montage);
	}
}

FTransform AGCharacter::GetLeftIKTransform_Implementation()
{
	if (Weapon)
	{
		FTransform SocketTransform = Weapon->SkeletalMeshComp->GetSocketTransform(FName("LeftHand_IK"));
		
		FVector BoneSpaceLocation;
		FRotator BoneSpaceRotation;
		GetMesh()->TransformToBoneSpace(FName("hand_r"), SocketTransform.GetLocation(), SocketTransform.Rotator(),BoneSpaceLocation, BoneSpaceRotation);
		FTransform BoneSpaceTransform;
		BoneSpaceTransform.SetLocation(BoneSpaceLocation);
		BoneSpaceTransform.SetRotation(BoneSpaceRotation.Quaternion());
		return BoneSpaceTransform;
	}
	return FTransform::Identity;
}

void AGCharacter::SpawnPrimaryWeapon_Implementation(TSubclassOf<AGBaseGun> WeaponClass)
{
	if (!Weapon)
	{
		
		SpawnWeaponLogic(WeaponClass);
		if (Weapon)
		{
			FAttachmentTransformRules AttachmentTransformRules = FAttachmentTransformRules::KeepRelativeTransform;
			AttachmentTransformRules.LocationRule = EAttachmentRule::SnapToTarget;
			AttachmentTransformRules.RotationRule = EAttachmentRule::SnapToTarget;
			AttachmentTransformRules.ScaleRule = EAttachmentRule::SnapToTarget;
			
			AttachmentTransformRules.bWeldSimulatedBodies = true;
			
		     Weapon->AttachToComponent(GetMesh(), AttachmentTransformRules, WeaponSocketBoneName);

			GetMesh()->SetAnimInstanceClass(GunAnimClass);

			GetCharacterMovement()->bUseControllerDesiredRotation = true;

			GetCharacterMovement()->bOrientRotationToMovement= false;

			AnimInstance = GetMesh()->GetAnimInstance();
			
			AnimationBinding();

			Weapon->GunSystem->SetUpVarFromDataAsset();
			
			Weapon->GunSystem->IsCharacterAI = IsCharacterAI;

			
			GunSystemBinding();
		}
	}

}


void AGCharacter::SwitchGunMode_Implementation()
{
}

void AGCharacter::Reloading_Implementation()
{
}

void AGCharacter::GunTriggerEnd_Implementation()
{
}

void AGCharacter::GunTrigger_Implementation()
{
}



void AGCharacter::OnMontageEnded_Implementation(UAnimMontage* Montage, bool Interrupted)
{
	if (!Interrupted)
	{
        MontageEnd();
		
	}
	else
	{
		MontageInterrupted();
	}
}

void AGCharacter::MontageEnd_Implementation()
{
	ActiveGameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("Status.Shooting"));
	ActiveGameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag("AnimGaurd.AnimPlaying"));
}

void AGCharacter::MontageInterrupted_Implementation()
{
	ActiveGameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("Status.Shooting")));

	ActiveGameplayTagContainer.RemoveTag(FGameplayTag::RequestGameplayTag(FName("AnimGaurd.AnimPlaying")));
}



void AGCharacter::HideWeapoon_Implementation(bool HidWeapon)
{
	if (Weapon)
	{
		Weapon->SkeletalMeshComp->SetHiddenInGame(HidWeapon);
	}
}

void AGCharacter::FunctionBinding_Implementation()
{
	InteractionSystemBinding();

	AnimationBinding();
}

void AGCharacter::CreateMainWidget_Implementation()
{
	
}






