// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "UObject/Interface.h"
#include "CharacterInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCharacterInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEAI_API ICharacterInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetMovementMode(EMovementMode MovementMode);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void PlayAnimation(FMontage& AnimMontage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void MontageStop(UAnimMontage* AnimMontage);
};
