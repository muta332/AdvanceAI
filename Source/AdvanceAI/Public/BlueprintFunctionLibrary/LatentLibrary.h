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
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, meta = (Latent, LatentInfo = "LatentInfo", WorldContext = "TargetActor"), Category = "TutorialLatent")
	static void MoveActorToLocation(
		AActor* TargetActor,
		FVector DesiredLocation,
		float Duration,
		FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, meta = (Latent, WorldContext = TargetActor, LatentInfo = LatentInfo), Category = "Delay")
	static void GDelay(FLatentActionInfo LatentInfo, AActor* TargetActor, float Duration);
	
};

class FLatentMoveActor : public FPendingLatentAction
{
public:;
	FName ExecutionFunction;
	int32 Linkage;
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
	Linkage(InLatentInfo.Linkage),
	TimeElasped(0.0)
	{
		//constructor code 
	}
	//Executes Everytick
	virtual void UpdateOperation(FLatentResponse& Response) override;
	
	
	
	
};



class FDelay : public FPendingLatentAction
{
public:
	FName FunctionExecution;
	int32 Linkage;
	FWeakObjectPtr CallBackTarget;
	AActor* WorldContextActor;
	float Duration;
	bool IsWaitTimeSet;
	float TimeRemaining;
	float WaitTime;
	FString WaitTimeText;
	FDelay(FName InFunctionExectionName, int32 InLinkage, FWeakObjectPtr InCallBackTarget, float InDuration, AActor* InWorldContextActor)
		:
		FunctionExecution(InFunctionExectionName),
		Linkage(InLinkage),
		CallBackTarget(InCallBackTarget),
		IsWaitTimeSet(false),
		Duration(InDuration),
	    WorldContextActor(InWorldContextActor)
	{
		// Contructor Code
	}
	virtual void UpdateOperation(FLatentResponse& Response) override;

	virtual FString GetDescription() const override;
	UFUNCTION()
	void Wait(float Duration);
};
