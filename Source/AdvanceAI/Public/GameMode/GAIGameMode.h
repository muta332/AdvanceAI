// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GAIGameMode.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class ADVANCEAI_API AGAIGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	void StartPlay() override;
	
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
