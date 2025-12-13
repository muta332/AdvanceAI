// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "GamePlayTagInterFaces.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UGamePlayTagInterFaces : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEAI_API IGamePlayTagInterFaces
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FGameplayTagContainer GetActiveGamePlayTagContainer();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddTag(const FGameplayTag& AddTag );

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveTag	(const FGameplayTag& RemoveTag );

	
};
