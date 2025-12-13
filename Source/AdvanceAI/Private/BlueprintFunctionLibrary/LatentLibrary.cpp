// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/LatentLibrary.h"

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
		Response.TriggerLink(ExecutionFunction, OutputLink, CallbackTarget);
		Response.DoneIf(true);
	}
	
}
