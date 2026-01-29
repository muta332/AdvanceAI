// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceAI/Public/Systems/GunSystem/GGunSystem.h"

#include <string>

#include "AIController.h"
#include "AdvanceAI/Public/InterFaces/GCharacterInterface.h"
#include "AdvanceAI/Public/BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/Systems/AttackSystem/AttackSystem.h"
#include "AdvanceAI/Macros.h"
#include "BlueprintFunctionLibrary/LatentLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ABasePlayerCharacter.h"
#include "AdvanceAI/Public/DataAsset/DA_GunSystem.h"
#include "InterFaces/GAIInterface.h"
#include "Perception/AISense_Hearing.h"




// Sets default values for this component's properties
UGGunSystem::UGGunSystem()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	FireRate = 0.1;

	AmmoInMag = 25;

	MagCapacity = 25;

	CurrentStorage = 100;

	BrustCount = 3;

	TraceLength = 90000;

	MaxAmmoStorage = CurrentStorage;

	SwitchCounter = -1;

	MaxSwitchCounter = 2;
}

void UGGunSystem::Triggered()
{
	FGameplayTagContainer Container = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(OwnerCharacter);
	if (Container.HasTag(FGameplayTag::RequestGameplayTag("Status.Dead"))) return;
	OnFireStarted.Broadcast();

	
	if (IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(OwnerCharacter).HasTag(FGameplayTag::RequestGameplayTag("AnimGaurd.AnimPlaying"))) return;
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this,&UGGunSystem::Attack, FireRate, true );
	
}

void UGGunSystem::TriggerReleased()
{
	FLatentActionInfo DelayInfo;
	DelayInfo.Linkage = 1;
	DelayInfo.CallbackTarget = this;
	DelayInfo.UUID = 123456;
	DelayInfo.ExecutionFunction = "TriggerReleaseFunction";
	ULatentLibrary::GDelay(DelayInfo, OwnerCharacter, FireRate);
	
}

void UGGunSystem::SwitchFiringModeCycle()
{
	if (OwnerCharacter->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(OwnerCharacter);
		
		if (!TagContainer.HasTag(ShootingStatus))
		{
			UGunBlueprintFunctionLibrary::DeleteTagBatch(OwnerCharacter, FiringModeTag);

			SwitchMode();
		}
	}
}

void UGGunSystem::Reloading()
{
	if (!(AmmoInMag == MagCapacity) && !(CurrentStorage < 0 || CurrentStorage == 0))
	{
		if (OwnerCharacter->Implements<UGCharacterInterface>())
		{
			IGCharacterInterface::Execute_PlayAnimation(OwnerCharacter, ReloadMontage);
		}
	}
	else if (CurrentStorage < 0 || CurrentStorage == 0)
	{
	   IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FGameplayTag::RequestGameplayTag("Gun.StorageEmty"));
	}
}


// Called when the game starts
void UGGunSystem::BeginPlay()
{
	Super::BeginPlay();

	SetUpVarFromDataAsset();

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
	{
		if (!OwnerCharacter) return;
		AsCharacter = Cast<AGCharacter>(OwnerCharacter);
		FName Name = OwnerCharacter->GetFName();

		AttackSystem = OwnerCharacter->FindComponentByClass<UAttackSystem>();
		if (AttackSystem)
		{
			PRINTONE("")
		}
	});

	

	
	
	
}

void UGGunSystem::ResetAttack()
{
	bHasFiredThisTrigger = false;
	BrustFireCounter = 0;
	
}

void UGGunSystem::SwitchOnFiringMode(const FGameplayTag& Tag)
{
	
	if (Tag == FiringModeAuto)
	{
		Fire();
	}
	else if (Tag == FiringModeBrust)
	{
		BurstFireFunction();
	}
	else if (Tag == FiringModeSingle)
	{
		SingleFireFunction();
	}
	
}

void UGGunSystem::Attack()
{
	if (AmmoInMag > 0 && OwnerCharacter && OwnerCharacter->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(OwnerCharacter);

		for (const FGameplayTag& Tag : TagContainer.GetGameplayTagArray())
		{
			if (Tag.MatchesTag(FiringModeTag))
			{
				SwitchOnFiringMode(Tag);
			}
		}
	}
	else if (AmmoInMag == 0 || AmmoInMag < 0 && OwnerCharacter)
	{
		IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter ,FGameplayTag::RequestGameplayTag("Gun.NeedToReload"));
	}


}

void UGGunSystem::SwitchAutoMode()
{
	UGunBlueprintFunctionLibrary::DeleteTagBatch(OwnerCharacter, FiringModeTag);
	IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FiringModeAuto);
}

void UGGunSystem::SwitchSingleMode()
{
	UGunBlueprintFunctionLibrary::DeleteTagBatch(OwnerCharacter, FiringModeTag);
	IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FiringModeSingle);
}

void UGGunSystem::SwitchBurstMode()
{
	UGunBlueprintFunctionLibrary::DeleteTagBatch(OwnerCharacter, FiringModeTag);
	IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FiringModeBrust);
}

void UGGunSystem::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (CVarGunSystem.GetValueOnGameThread() && OwnerCharacter)
	{
		int32 ID = GetUniqueID();
		FString AmmosInMag = FString::FromInt(AmmoInMag);
		FString MagsCapacity = FString::FromInt(MagCapacity);
		FString DebugMsg = OwnerCharacter->GetName() + " = " ;
		PRINT(ID, 0, FColor::Green, DebugMsg )
	}
}

void UGGunSystem::SetUpVarFromDataAsset()
{
	if (!GunAsset) return;
	FireRate = GunAsset->FireRate;
	AmmoInMag = GunAsset->AmmoInMag;
	MagCapacity = GunAsset->MagCapacity;
	BrustCount = GunAsset->BrustCount;
	GunShotSound = GunAsset->GunShotSound;
	DamageInfo = GunAsset->DamageInfo;
	ProjectileMuzzleName = GunAsset->ProjectileMuzzleName;
	ReloadMontage = GunAsset->ReloadMontage;
	MuzzleParticleSystem = GunAsset->MuzzleParticleSystem;
	MaxAmmoStorage = GunAsset->MaxAmmoStorage;
	CurrentStorage = GunAsset->CurrentStorage;
	OwnerCharacter = GetOwner()->GetOwner();
	AttackMontage = GunAsset->AttackMontage;
	MuzzleEmitterSclae = GunAsset->GunMuzzleEmitterScale;
	
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
	if (AttackSystem)
	{
		FProjectileInfo ProjectileInfo;
		ProjectileInfo.DamageInfo = DamageInfo;
		ProjectileInfo.BP_ProjectileClass = GunAsset->ProjectileClass;
		ProjectileInfo.DestroyedEmitter = GunAsset->MuzzleParticleSystem;
		ProjectileInfo.InitialSpeed =  GunAsset->BulletSpawnData.InitialSpeed;
		ProjectileInfo.Gravity = BulletSpawnData.Gravity;
		ProjectileInfo.bRecoil = BulletSpawnData.bRecoil;
		ProjectileInfo.DestroyedEmitterScale = GunAsset->BulletSpawnData.DestroyedEmitterScale;
		ProjectileInfo.DamageInfo.DamageCauser = OwnerCharacter;

		FRotator BulletRotation;
		
		UGunBlueprintFunctionLibrary::CameraAim(AsCharacter, TraceLength, ProjectileMuzzleName, ProjectileInfo.TraceEnd, ProjectileInfo.SpawnStart, ProjectileInfo.TraceStart,BulletRotation );
		if (IsCharacterAI && OwnerCharacter)
		{
			ProjectileInfo.TraceStart = OwnerCharacter->GetActorLocation();
			if (!IGAIInterface::Execute_GetTargetActor(OwnerCharacter)) return;
			ProjectileInfo.TraceEnd = IGAIInterface::Execute_GetTargetActor(OwnerCharacter)->GetActorLocation();
		}
		AttackSystem->ProjectileAttack(ProjectileInfo);

		UpdateMag();
	}
}

void UGGunSystem::UpdateMag()
{
	if (AmmoInMag > 0)
	{
		AmmoInMag--;
		
		OnUpdateMag.Broadcast(AmmoInMag);
		
	}
}

void UGGunSystem::SwitchMode()
{
	
	switch (FiringMode)
	{
	case EFiringMode::EFM_Single:
		FiringMode = EFiringMode::EFM_Auto;
		IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FiringModeAuto);
		break;
	case EFiringMode::EFM_Auto:
		FiringMode = EFiringMode::EFM_Burst;
		IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FiringModeBrust);
		break;

	case EFiringMode::EFM_Burst:
		FiringMode = EFiringMode::EFM_Single;
		IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter,FiringModeSingle);
		break;
	}
}




void UGGunSystem::OnReloadAmmoUpdate()
{
	UsedBullets = MagCapacity - AmmoInMag;
	

	
	
	
	if (CurrentStorage > MagCapacity)
	{
		CurrentStorage = FMath::Clamp(CurrentStorage-UsedBullets, 0, MaxAmmoStorage);
		AmmoInMag = FMath::Clamp(MagCapacity, 0, MagCapacity);
	}
	else
	{
		AmmoInMag = FMath::Clamp(CurrentStorage + AmmoInMag, 0, MagCapacity);
		CurrentStorage = FMath::Clamp(CurrentStorage-UsedBullets, 0, MaxAmmoStorage);
	}
	


	

	OnReloadFinished.Broadcast(AmmoInMag, CurrentStorage);
}

void UGGunSystem::SetAmmoStorage(int32 Amount)
{
	CurrentStorage = FMath::Clamp(Amount + CurrentStorage, 0.f, MaxAmmoStorage);

	UpdateCurrentStorage.Broadcast(CurrentStorage);
}

void UGGunSystem::TriggerReleaseFunction()
{

	ResetAttack();
	AAIController;
	
	GetWorld()->GetTimerManager().ClearTimer(AttackTimerHandle);

	
	OnFireEnded.Broadcast();

	if (OwnerCharacter && OwnerCharacter->Implements<UGCharacterInterface>())
	{
		IGCharacterInterface::Execute_MontageStop(OwnerCharacter, AttackMontage.MontageToPlay);
	}
	if ((AmmoInMag < 0 || AmmoInMag == 0) && !(CurrentStorage < 0 || CurrentStorage == 0) && OwnerCharacter->Implements<UGCharacterInterface>())
	{
		IGCharacterInterface::Execute_PlayAnimation(OwnerCharacter, ReloadMontage );
	}
	
	
	
	

	
}

void UGGunSystem::BurstFireFunction()
{
	if (BrustFireCounter < BrustCount || BrustFireCounter == BrustCount)
	{
		Fire();
		BrustFireCounter++;
	}
		
}

void UGGunSystem::SingleFireFunction()
{
	if (bHasFiredThisTrigger == false)
	{
		bHasFiredThisTrigger = true;
		Fire();
		
		
		
	}
	
	
}

void UGGunSystem::Fire()
{
	
	if (!(OwnerCharacter && GunShotSound && GunSkeletalMesh && MuzzleParticleSystem)) return;
	AttackAssistantFunction();

	IGamePlayTagInterFaces::Execute_AddTag(OwnerCharacter, ShootingStatus);

	IGCharacterInterface::Execute_PlayAnimation(OwnerCharacter, AttackMontage);

	UGameplayStatics::PlaySoundAtLocation(OwnerCharacter, GunShotSound, GunSkeletalMesh->GetSocketLocation(ProjectileMuzzleName));
	
	UAISense_Hearing::ReportNoiseEvent(this->GetWorld(),  GunSkeletalMesh->GetSocketLocation(ProjectileMuzzleName), 1, OwnerCharacter);

	UGameplayStatics::SpawnEmitterAtLocation(OwnerCharacter, MuzzleParticleSystem, GunSkeletalMesh->GetSocketLocation(ProjectileMuzzleName), FRotator::ZeroRotator, FVector(MuzzleEmitterSclae));

	

	
}

void UGGunSystem::GetUsedAmmoAmountInStorage(int32& ReturnValue)
{
	if (MaxAmmoStorage != CurrentStorage)
	{
		ReturnValue =  MaxAmmoStorage-CurrentStorage;
	}
	
}





