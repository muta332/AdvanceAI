// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AdvanceAI/Public/BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/Projectile/GProjectileBaseClass.h"
#include "DA_GunSystem.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ADVANCEAI_API UDA_GunSystem : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	//Current bullets in Magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 AmmoInMag;

	//Current bullets in Magazine
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MagCapacity;

	//Remaining Ammo In Inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 CurrentStorage;

	//Max Ammo, Character Allowed to Carry
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmoStorage;

	//Max Distance where the End of Trace is
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceLength;

	//Fire Rate of the gunj. 1/60 = 60 shots per second
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireRate;
	

	//How many shots per tirgger for brust fire Only
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 BrustCount;
	

	//Information of how damage is inflicted
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDamageInfo DamageInfo;

	//Name of the Bone where bullets discharage
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	FName ProjectileMuzzleName;
	
	//Info of spawned bullet to reach its destination
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FProjectileInfo BulletSpawnData;

	//Attack Montage Related Information
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMontage AttackMontage;


	//Information for Reload Anim Montage
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FMontage ReloadMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* GunShotSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Muzzle")
	UParticleSystem* MuzzleParticleSystem;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ammo")
	TSubclassOf<AGProjectileBaseClass> ProjectileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Muzzle")
	float GunMuzzleEmitterScale;

	

	
	
};
