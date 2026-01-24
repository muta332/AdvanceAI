// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "AsyncActionLibrary.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam (FUpdateinterval, int, IntervalCount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelayCompleted);
UCLASS()
class ADVANCEAI_API UAsyncActionLibrary : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
	UPROPERTY(BlueprintAssignable)
	FUpdateinterval OnInterval;

	UPROPERTY(BlueprintAssignable)
	FDelayCompleted OnFinished;

	UFUNCTION(BlueprintCallable ,meta = (WorldContext = "WorldContextObject"))
	static UAsyncActionLibrary* DelayWithInterval(UObject* WorldContextObject, float Interval, float Duration);

	virtual void Activate() override;

private :

	FTimerHandle timerHandleLoop;
	FTimerHandle TimerHandleFinish;

	UPROPERTY()
	UObject* WorldContextObject;

	float Interval;
	float Duration;

	int32 IntervalCount;

	void Finish();
};
