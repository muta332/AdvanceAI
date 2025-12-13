// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "GunBlueprintFunctionLibrary.generated.h"


/**
 * 
 */
USTRUCT(BlueprintType)
struct FDamageInfo
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	float Amount;

	UPROPERTY(BlueprintReadWrite)
	bool bShouldDamageInvinsible;

	UPROPERTY(BlueprintReadWrite)
	bool CanBeBlocked;

	UPROPERTY(BlueprintReadWrite)
	bool CanBeParried;

	UPROPERTY(BlueprintReadWrite)
	bool ShouldForceInterrupt;
	
};

USTRUCT(BlueprintType)
struct FMontage
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	UAnimMontage* MontageToPlay;

	UPROPERTY(BlueprintReadWrite)
	float InPlayRate;

	UPROPERTY(BlueprintReadWrite)
	FName SectionName;

	UPROPERTY(BlueprintReadWrite)
	bool bStopMontages;

	
};

USTRUCT(BlueprintType)
struct FProjectileInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FVector TraceStart;

	UPROPERTY(BlueprintReadWrite)
	FVector TraceEnd;

	UPROPERTY(BlueprintReadWrite)
	FVector SpawnStart;

	UPROPERTY(BlueprintReadWrite)
	FVector SpawnEnd;

	UPROPERTY(BlueprintReadWrite)
	FDamageInfo DamageInfo;

	UPROPERTY(BlueprintReadWrite)
	AActor* BP_ProjectileClass;

	UPROPERTY(BlueprintReadWrite)
	UParticleSystem* ParticleSystem;

	UPROPERTY(BlueprintReadWrite)
	float InitialSpeed;

	UPROPERTY(BlueprintReadWrite)
	float Gravity;

	
};
UCLASS()
class ADVANCEAI_API UGunBlueprintFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	static void DeleteTagBatch(AActor* Instigator, FGameplayTag ParentTag);
};


