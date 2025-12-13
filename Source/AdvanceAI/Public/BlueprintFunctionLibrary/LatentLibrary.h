// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "LatentLibrary.generated.h"

/**
 * 
 */
UCLASS()
class ADVANCEAI_API ULatentLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "TargetActor"), Category = "TutorialLatent")
	static void MoveActorToLocation(
		AActor* TargetActor,
		FVector DesiredLocation,
		float Duration,
		FLatentActionInfo LatentInfo);
	
};

class FLatentMoveActor : public FPendingLatentAction
{
public:;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;
	TWeakObjectPtr<AActor> TargetActor;
	FVector StartLocation;
	FVector DesiredLocation;
	float Duration;
	float TimeElasped;

	

	FLatentMoveActor(
		AActor* InTargetActor,
		FVector InDesiredLocation,
		float InDuration,
		const FLatentActionInfo& InLatentInfo)
	:
	StartLocation(InTargetActor->GetActorLocation()),
	DesiredLocation(InDesiredLocation),
	Duration(InDuration),
	TargetActor(InTargetActor),
	ExecutionFunction(InLatentInfo.ExecutionFunction),
	CallbackTarget(InLatentInfo.CallbackTarget),
	OutputLink(InLatentInfo.Linkage),
	TimeElasped(0.0)
	{
		//constructor code
	}
	virtual void UpdateOperation(FLatentResponse& Response) override;
	
	
	
	
};
