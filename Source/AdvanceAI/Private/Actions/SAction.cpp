// Fill out your copyright notice in the Description page of Project Settings.


#include "AdvanceAI/Public/Actions/SAction.h"

UWorld* USAction::GetWorld() const
{
	UActorComponent* comp = Cast<UActorComponent>(GetOuter());
	if (comp)
	{
		return comp->GetOwner()->GetWorld();
	}
	return nullptr;
}