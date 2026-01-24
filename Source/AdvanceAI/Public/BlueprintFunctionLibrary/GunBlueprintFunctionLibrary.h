// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "Camera/CameraComponent.h"
#include "AdvanceAI/Public/Projectile/GProjectileBaseClass.h"
#include "GunBlueprintFunctionLibrary.generated.h"


/**
 * 
 */
UENUM(BlueprintType, Blueprintable)
enum class EAIMode: uint8
{
	EAM_BattleMode UMETA(DisplayName = "Battle Mode"),
	EAM_PassiveMode UMETA(DisplayName = "Passive Mode"),
	EAM_ConfusionMode UMETA(DisplayName = "Confusion Mode")
};
//Enum for AI Sight, hearing and Damage Sense
UENUM(BlueprintType)
enum class EAISense: uint8
{
     EAS_Sight UMETA(DisplayName = "Sight"),
	EAS_Hearing UMETA(DisplayName = "Hearing"),
	EAS_Damage UMETA(DisplayName = "Damage")
};

//Used by ABP to dermine when to play Walk, run, Sprint and Crouch
UENUM(BlueprintType)
enum class EStanceMode : uint8
{
	ESM_Running UMETA(DisplayName = "Running"),
	ESM_Sprinting UMETA(DisplayName = "Sprinting"),
	ESM_Crouching UMETA(DisplayName = "Crouching"),
	ESM_Walking UMETA(DisplayName = "Walking")
};

//Used to Describe What Type of, amount, speciality of Damage inflicted upon enemy
UENUM(BlueprintType)
enum class EDamageType : uint8
{
	EDT_None UMETA(DisplayName = "None"),
	EDT_Projectile UMETA(DisplayName = "Projectile"),
	EDT_Explosion UMETA(DisplayName = "Explosion"), 
	EDT_Environment UMETA(DisplayName = "Environment"),
	EDT_Melee UMETA(DisplayName = "Melee")
};

//Used to Describe What kind of Response after inflicted with damage
UENUM(BlueprintType)
enum class EDamageResponse : uint8
{
	EDR_None UMETA(DisplayName = "None"),
	EDR_HitReaction UMETA(DisplayName = "HitReaction"),
	EDR_Stunned UMETA(DisplayName = "Stunned"),
	EDR_Stagger UMETA(DisplayName = "Stagger"),
	EDR_Knockback UMETA(DisplayName = "Knockback"),
	
};

// Stores an attack's Damage type like how much, anim of damage, type, in response
USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageType DamageType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EDamageResponse DamageResponse;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Amount;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShouldDamageInvinsible;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool CanBeBlocked;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool CanBeParried;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool ShouldForceInterrupt;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AActor* DamageCauser;
	
};

//Stores an Animations Details. What Anim, section, rate of the animation
USTRUCT(BlueprintType)
struct FMontage
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UAnimMontage* MontageToPlay;



	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float InPlayRate;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName SectionName;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bStopMontages;

	
};

//Stores a projectile's like bullet or magic bolt start, end location ,Spawn rotation, projectile class etc
USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector TraceStart;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector TraceEnd;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector SpawnStart;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FDamageInfo DamageInfo;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
    TSubclassOf<AGProjectileBaseClass> BP_ProjectileClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UParticleSystem* DestroyedEmitter;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float InitialSpeed;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Gravity;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float DestroyedEmitterScale;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bRecoil;
	
};



//Main BlueprintFunctionLibrary Class
UCLASS(Blueprintable)
class ADVANCEAI_API UGunBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	//Provide the parent tag and tag container. will delete all the child tag in the container of the provided container
	UFUNCTION(BlueprintCallable)
	static void DeleteTagBatch(AActor* Instigator, FGameplayTag ParentTag);

	//provideds rotation and spawn location for projectile type attack
    UFUNCTION(BlueprintCallable, BlueprintPure)
	static void CameraAim(
		//Input
		ACharacter* Instigator,
		float CameraTraceLength,
		FName SpawnLocationBoneName,
		//Output
		FVector& EndLocation,
		FVector& SpawnStart,
		FVector& StartLocation,
		FRotator& SpawnRotation
	);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static void GetDistanceBetweenTwoActors(AActor* ActorOne, AActor* ActorTwo, float& Distance);

	static bool DoesActorImplementInterFace(AActor* Actor, TSubclassOf<UInterface> InterfaceClass);

	
};


