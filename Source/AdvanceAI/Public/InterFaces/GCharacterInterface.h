// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "UObject/Interface.h"
#include "Weapon/Gun/GBaseGun.h"
#include "GCharacterInterface.generated.h"
class AGCharacter;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType, Blueprintable)
class UGCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEAI_API IGCharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetMovementMode(EMovementMode MovementMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayAnimation(const FMontage& AnimMontage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MontageStop(UAnimMontage* AnimMontage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	AGCharacter* GetCharacter();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void StorageInteraction(const int32& UpdateAmmo);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void EquipWeapon(TSubclassOf<AGBaseGun> Weapon);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	int32 GetTeamNumber();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CheckifHostile(AActor* OtherActor);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void GetWeapon(AGBaseGun*& AsWeapon);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetSpeedBasedOnStanceTag(const FGameplayTag StanceTag);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool TakeDamage(const FDamageInfo& DamageInfo);
};
