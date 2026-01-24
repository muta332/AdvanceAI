// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UGAIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEAI_API IGAIInterface
{
	GENERATED_BODY()
  
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FVector GetSplineLocation();

	//Call from AI Controller to Set the TargetActor Value of Base AI Enemy
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetTargetActor(AActor* TargetActor);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	AActor* GetTargetActor();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GetGunIdentificationTagName(FGameplayTag& GunTag);
};
