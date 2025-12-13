// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GameplayTagContainer.h"
#include "GCharacter.generated.h"

UCLASS()
class ADVANCEAI_API AGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AGCharacter();

	protected:
	UFUNCTION(BlueprintCallable)
	virtual void SpawnWeapon();

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer ActiveGameplayTagContainer;
};
