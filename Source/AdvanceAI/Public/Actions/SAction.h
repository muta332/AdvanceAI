// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SAction.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ADVANCEAI_API USAction : public UObject
{
	GENERATED_BODY()
protected:
	UWorld* GetWorld() const override;
	
	
};


