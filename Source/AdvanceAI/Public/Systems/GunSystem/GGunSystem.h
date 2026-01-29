// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/InterFaces/GamePlayTagInterFaces.h"
#include "GGunSystem.generated.h"
class  UDA_GunSystem;
class UAttackSystem;
class UParticleSystem;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateCurrentStorage, int32, CurrentStorage);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFireEnded);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnReloadFinished, const int32&, Ammo, const int32&, StorageAmmo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateMag, const int32&, Ammo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FUpdateNewCurrentStorage, int32, CurrentStorage);
static TAutoConsoleVariable<bool> CVarGunSystem (TEXT("su.system.Gun"), false,TEXT("if true, will show information of currenty equiped weapon"));

UENUM(BlueprintType)
enum class EFiringMode: uint8
{
	EFM_Single UMETA(DisplayName = "Single"),
	EFM_Auto UMETA(DisplayName = "Auto"),
	EFM_Burst UMETA(DisplayName = "Burst")
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable )
class ADVANCEAI_API UGGunSystem : public UActorComponent
{
	GENERATED_BODY()

public:
	//Delegates
	UPROPERTY(BlueprintAssignable)
	FUpdateCurrentStorage UpdateCurrentStorage;

	UPROPERTY(BlueprintAssignable)
	FOnFireStarted OnFireStarted;
	
	UPROPERTY(BlueprintAssignable)
	FOnFireEnded OnFireEnded;
	
	UPROPERTY(BlueprintAssignable)
	FOnReloadFinished OnReloadFinished;
	UPROPERTY(BlueprintAssignable)
	FOnUpdateMag OnUpdateMag;
	
	UPROPERTY(BlueprintAssignable)
	FUpdateNewCurrentStorage UpdateNewCurrentStorage;
	
	//Skeletal Mesh component of the gun
	UPROPERTY(BlueprintReadWrite, Category = "Components")
	USkeletalMeshComponent* GunSkeletalMesh;

	//Current bullets in Magazine
	UPROPERTY(BlueprintReadWrite, Category = "Ammo", VisibleInstanceOnly)
	int32 AmmoInMag;

	//Current bullets in Magazine
	UPROPERTY(BlueprintReadWrite, Category = "Ammo", VisibleInstanceOnly)
	int32 MagCapacity;

	//Remaining Ammo In Inventory
	UPROPERTY(BlueprintReadWrite, Category = "Ammo", VisibleInstanceOnly)
	int32 CurrentStorage;

	//Max Ammo, Character Allowed to Carry
	UPROPERTY(BlueprintReadWrite, Category = "Ammo", VisibleInstanceOnly)
	int32 MaxAmmoStorage;

	//Max Distance where the End of Trace is
	UPROPERTY(BlueprintReadWrite)
	float TraceLength;

	//Fire Rate of the gunj. 1/60 = 60 shots per second
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	float FireRate;

	//The Playable Character or AI
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<ACharacter> AsCharacter;

	//How many shots per tirgger for brust fire Only
	UPROPERTY(BlueprintReadWrite)
	int32 BrustCount;



	//Information of how damage is inflicted
	UPROPERTY(BlueprintReadWrite)
	FDamageInfo DamageInfo;

	//Name of the Bone where bullets discharage
	UPROPERTY(BlueprintReadWrite)
	FName ProjectileMuzzleName;

	UPROPERTY(BlueprintReadWrite)
	FTimerHandle AttackTimer;

	//Info of spawned bullet to reach its destination
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	FProjectileInfo BulletSpawnData;

	//Attack Montage Related Information
	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly)
	FMontage AttackMontage;


	//Information for Reload Anim Montage
	UPROPERTY(BlueprintReadWrite)
	FMontage ReloadMontage;

	UPROPERTY(BlueprintReadWrite, Category = "Character")
	AActor* OwnerCharacter;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsCharacterAI = false;
	
protected:
	//Counter For Advance Flip Flop goes more than one
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 SwitchCounter;

	//Max Number for Advance flip flop
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	int32 MaxSwitchCounter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tag")
	FGameplayTag CameraZoomTag = FGameplayTag::RequestGameplayTag("Camera.ZoomIn");

	//Used to clear the loop function which executes Attack function
	UPROPERTY(BlueprintReadWrite, Category = "Timer")
	FTimerHandle AttackTimerHandle;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tag")
	FGameplayTag ShootingStatus = FGameplayTag::RequestGameplayTag("Status.Shooting");

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeTag = FGameplayTag::RequestGameplayTag("FiringMode");

	//BPC Attack System Actor Component
	UPROPERTY(BlueprintReadWrite, Category = "Component")
	UAttackSystem* AttackSystem;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeAuto = FGameplayTag::RequestGameplayTag("FiringMode.Auto");

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeBrust = FGameplayTag::RequestGameplayTag("FiringMode.Brust");

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Tag")
	FGameplayTag FiringModeSingle = FGameplayTag::RequestGameplayTag("FiringMode.Single");
	
	UPROPERTY(BlueprintReadWrite, Category = "Sound", VisibleInstanceOnly)
	USoundBase* GunShotSound;

	//Scales the spawned priticle in gun Muzzle
	UPROPERTY(BlueprintReadWrite)
	float MuzzleEmitterSclae;

	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* MuzzleParticleSystem;

	//Data asset for Guns that sets all the critical variables for a gun to function in this class
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = "Data Asset")
	 UDA_GunSystem* GunAsset;

	//Class of the Guns Bullet/Projectile
	UPROPERTY(BlueprintReadOnly, Category = "Ammo")
	TSubclassOf<AActor> ProjectileClass;

	//Amount used In a Full Mag. Max-Current in a mag
	UPROPERTY(BlueprintReadWrite, Category = "Ammo")
	float UsedBullets;

	
	EFiringMode FiringMode = EFiringMode::EFM_Single;
	

	

	

private:
	int32 BrustFireCounter = 0;

	bool bHasFiredThisTrigger = false;
	
	

	


public:
	//For Player and AI Usage
	UGGunSystem();

	//Call to pull Trigger
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void Triggered();

	//Call to stop pulling the trigger
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void TriggerReleased();

	
    //call to reload
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void Reloading();

	//call to setup component variables from DataAsset
	UFUNCTION(BlueprintCallable, Category = "DataAsset")
	virtual void SetUpVarFromDataAsset();
	
	//call to Switch gun mode between Auto, Single and burst mode. 
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void SwitchFiringModeCycle();

	

	

protected:
	//For Componet Usage. Not Callable Directly for Players or AI. All of these functions are internal usage only
	
	//Call to Switch Mode of the gun
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void SwitchMode();
	
	
	
	virtual void BeginPlay() override;

	//Called by TriggerReleased to reset the Trigger
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void ResetAttack();
	void SwitchOnFiringMode(const FGameplayTag& Tag);

	//called by Trigger. Loops every FireRate Interval 
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void Attack();

	//Call to manually set the gun to auto mode instead of cycling to Auto mode
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	void SwitchAutoMode();

	//Call to manually set the gun to Single mode instead of cycling to Single mode
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	void SwitchSingleMode();

	//Call to manually set the gun to Burst mode instead of cycling to Burst mode
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	void SwitchBurstMode();

	//Ticks EveryFrams
	void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	


//Not Used
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void AttackAmmoCalculation();

	//Not used 
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual FProjectileInfo CameraAim();

	//Calls to initiate the projectile Spawn data, calls the update mag function and does necessary trace for AI and player to spawn bullet
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void AttackAssistantFunction();

	//Updates the Mag each time gun fires also calls the OnUpdate Mag
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void UpdateMag();

	
	

	

	//Calls when reload finished. updates necessary variables and call OnReload delegate
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void OnReloadAmmoUpdate();

	//call to update the ammo storage when more ammo collected from other ammo sources also exec UpdateCurrentStorage delegate
	UFUNCTION(BlueprintCallable, Category = "GunSystem")
	virtual void SetAmmoStorage(int32 Amount);

	//called by TriggerReleased
	UFUNCTION()
	virtual void TriggerReleaseFunction();

	//Calls to fire when the Gun is in BurstMode
	void BurstFireFunction();

	//Calls to fire when the Gun is in SingleMode
	void SingleFireFunction();

	//Calls to Fire
	void Fire();

	//Call to get the ammount of ammo used up. Besically max ammo - used ammo in the storage
	UFUNCTION(BlueprintCallable, BlueprintPure)
	void GetUsedAmmoAmountInStorage(int32& ReturnValue);
	

	
	

	
		
};

