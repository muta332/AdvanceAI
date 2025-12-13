// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/InterFaces/GamePlayTagInterFaces.h"
#include "GGunSystem.generated.h"

class UAttackSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireStarted);


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEnded);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReloadFinished, int32, Ammo, int32, StorageAmmo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateAmmo, int32, Ammo);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEAI_API UGGunSystem : public UActorComponent
{
	GENERATED_BODY()

public:	

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 Ammo;

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmo;

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 AmmoStorage;

	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 MaxAmmoStorage;

	UPROPERTY(BlueprintReadWrite)
	float TraceLength;

	UPROPERTY(BlueprintReadWrite)
	float FireRate;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> AsCharacter;

	UPROPERTY(BlueprintReadWrite)
	int32 BrustCount;

	UPROPERTY(BlueprintReadWrite)
	bool bIsAI;

	UPROPERTY(BlueprintReadWrite)
	FDamageInfo DamageInfo;

	UPROPERTY(BlueprintReadWrite)
	FName ProjectileMuzzleName;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle AttackTimer;

	UPROPERTY(BlueprintReadWrite)
	FProjectileInfo BulletSpawnData;

	UPROPERTY(BlueprintReadWrite)
	FMontage AttackMontage;

	UPROPERTY(BlueprintReadWrite)
	FMontage ReloadMontage;

	UPROPERTY(BlueprintReadOnly)
	FOnFireStarted OnFireStarted;

	UPROPERTY(BlueprintReadOnly)
	FOnFireEnded OnFireEnded;

	UPROPERTY(BlueprintReadOnly)
	FOnReloadFinished OnReloadEnded;

	UPROPERTY(BlueprintReadOnly)
	FOnUpdateAmmo OnUpdateAmmo;
	
protected:
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 LoopCounterForRiflingMode;
	
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 MaxLoopCounterForRiflingMode;

	UPROPERTY(BlueprintReadWrite, Category = "Tag")
	FGameplayTag CameraZoomTag;

	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	FTimerHandle AttackTimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "Tag")
	FGameplayTag ShootingStatus;

	UPROPERTY(BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeTag;

	UPROPERTY(BlueprintReadWrite, Category = "Component")
	UAttackSystem* AttackSystem;

	UPROPERTY(BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeAuto;

	UPROPERTY(BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeBrust;

	UPROPERTY(BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeSingle;
	

	

	


public:	
	UGGunSystem();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void Triggered();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void TriggerReleased();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void SwitchFiringMode();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void Reloading();

	

	

protected:
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void ResetAttack();
	void SwitchOnFiringMode(const FGameplayTag& Tag);

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void Attack();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void AttackAmmoCalculation();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual FProjectileInfo CameraAim();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void AttackAssistantFunction();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void AmmoUpdate();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void SwitchMode();
	void Shoot();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void ModeAutoFunction();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void ModeBurstFunction();

	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void ModeSingleFunction();

	void  DoN(int32 N);

	

	
		
};

