// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/LatentLibrary.h"

#include <string>

void ULatentLibrary::MoveActorToLocation(AActor* TargetActor, FVector DesiredLocation, float Duration,
                                         FLatentActionInfo LatentInfo)
{
	if (!TargetActor)
	{
		return;
	}
	if (UWorld* World = GEngine->GetWorldFromContextObjectChecked(TargetActor))
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FLatentMoveActor>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FLatentMoveActor(TargetActor, DesiredLocation, Duration, LatentInfo));
		}
	}
	
}

void ULatentLibrary::GDelay(FLatentActionInfo LatentInfo, AActor* TargetActor, float Duration)
{
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(TargetActor);
	if (World)
	{
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FDelay>(LatentInfo.CallbackTarget, LatentInfo.UUID) == nullptr)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FDelay(LatentInfo.ExecutionFunction, LatentInfo.Linkage, LatentInfo.CallbackTarget, Duration,TargetActor));
		}
	}
}
void FDelay::UpdateOperation(FLatentResponse& Response)
{
	if (IsWaitTimeSet == false)
	{
		WaitTime = WorldContextActor->GetWorld()->GetTimeSeconds() + Duration;
		IsWaitTimeSet = true;
	}
	TimeRemaining = WaitTime - WorldContextActor->GetWorld()->GetTimeSeconds();
	GetDescription();
	
	if (TimeRemaining == 0 || TimeRemaining < 0)
	{
		Response.TriggerLink(FunctionExecution, Linkage, CallBackTarget);
		Response.DoneIf(true);
	}
}

FString FDelay::GetDescription() const
{
	
	return FString::Printf(TEXT("%.2f"), TimeRemaining);
}


void FLatentMoveActor::UpdateOperation(FLatentResponse& Response)
{
	FPendingLatentAction::UpdateOperation(Response);
	if (!TargetActor.IsValid() )
	{
		Response.DoneIf(true);
		return;
	}
	TimeElasped += Response.ElapsedTime();
	float Alpha = FMath::Clamp(Response.ElapsedTime()/ Duration, 0.0f, 1.0f);
	FVector NewLocation = FMath::Lerp(StartLocation, DesiredLocation, Alpha);

	TargetActor->SetActorLocation(NewLocation);

	if (Alpha>= 1.0f)
	{
		Response.TriggerLink(ExecutionFunction, Linkage, CallbackTarget);
		Response.DoneIf(true);
		//Response.FinishAndTriggerIf();
	}
	
}


