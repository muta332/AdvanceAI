// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/AsyncActionLibrary.h"

UAsyncActionLibrary* UAsyncActionLibrary::DelayWithInterval(UObject* WorldContextObject, float Interval, float Duration)
{
    UAsyncActionLibrary* AsyncTask = NewObject<UAsyncActionLibrary>();
    AsyncTask->WorldContextObject = WorldContextObject;
    AsyncTask->Interval = Interval;
    AsyncTask->Duration = Duration;
    return AsyncTask;
}

void UAsyncActionLibrary::Activate()
{
    Super::Activate();
    if (WorldContextObject)
    {
        WorldContextObject->GetWorld()->GetTimerManager().SetTimer(timerHandleLoop, [this]()
        {
            OnInterval.Broadcast(IntervalCount);
            IntervalCount++;
        }, Interval, true);

        WorldContextObject->GetWorld()->GetTimerManager().SetTimer(TimerHandleFinish, [this]()
        {
            OnFinished.Broadcast();
            Finish();
        }, Duration, false );
    }
    else
    {
        Finish();
    }
}

void UAsyncActionLibrary::Finish()
{
    if (WorldContextObject)
    {
        UWorld* World = WorldContextObject->GetWorld();

        if (World)
        {
            World->GetTimerManager().ClearTimer(TimerHandleFinish);
            World->GetTimerManager().ClearTimer(timerHandleLoop);
        }
        OnFinished.Broadcast();
        SetReadyToDestroy();
    }
}

