// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/InterFaces/GamePlayTagInterFaces.h"

void UGunBlueprintFunctionLibrary::DeleteTagBatch(AActor* Instigator, FGameplayTag ParentTag)
{
	if (Instigator->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(Instigator);
		

		for (FGameplayTag Tag : TagContainer.GetGameplayTagArray())
		{
			if (Tag.MatchesTag(ParentTag))
			{
				IGamePlayTagInterFaces::Execute_RemoveTag(Instigator, Tag);
			}
		}
	}
}



