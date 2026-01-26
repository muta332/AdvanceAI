// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GAIControllerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UGAIControllerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ADVANCEAI_API IGAIControllerInterface
{
	GENERATED_BODY()
public: 
	UFUNCTION( BlueprintCallable, BlueprintNativeEvent)
	void SetValueAsObject(UObject* Object, FName KeyName);

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
};
