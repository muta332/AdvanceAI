// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/WeaponHandling/GBTTask_Reload.h"

#include "AIController.h"

#include "AdvanceAI/Public/Systems/GunSystem/GGunSystem.h"
#include "AdvanceAI/Public/InterFaces/GCharacterInterface.h"
#include "Player/GCharacter.h"

void UGBTTask_Reload::AbortFunctionIfFailedToReload()
{
	GetWorld()->GetTimerManager().SetTimer(AbortFunctionTimerHandle,  [this]()
	{
		if (bFunctionRunning)
		{
			FinishLatentTask(*OwnerComponent, EBTNodeResult::Failed);
			bFunctionRunning = false;
		}
	},AbortTimer, true );
}

void UGBTTask_Reload::DoReloadTask(AGCharacter* AI)
{
	if (GunSystem)
	{
		GunSystem->TriggerReleased();
			
		GunSystem->Reloading();
		
		
			
		AbortFunctionIfFailedToReload();
			
		Character = IGCharacterInterface::Execute_GetCharacter(AI);
			
		if (Character)
		{
			Character->AnimInstance->OnMontageEnded.AddDynamic(this, &UGBTTask_Reload::MontageEnded);
		}
	}
}

EBTNodeResult::Type UGBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!(OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())) return EBTNodeResult::Failed;
	bFunctionRunning = true;
	
	
	AGBaseGun* WeaponActor;
	
	OwnerComponent = &OwnerComp;
	
	AGCharacter* AI= Cast<AGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	IGCharacterInterface::Execute_GetWeapon(AI, WeaponActor);
	
	if (WeaponActor)
	{
		GunSystem = WeaponActor->FindComponentByClass<UGGunSystem>();
		
		DoReloadTask(AI);
		
		return EBTNodeResult::InProgress;
	}
	
	
	
	return EBTNodeResult::InProgress;
}

void UGBTTask_Reload::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bFunctionRunning = false;
	
	Character->AnimInstance->OnMontageEnded.RemoveDynamic(this, &UGBTTask_Reload::MontageEnded);
	
	if (!bInterrupted)
	{
        FinishLatentTask(*OwnerComponent, EBTNodeResult::Succeeded);
	}
	else
	{
		 FinishLatentTask(*OwnerComponent, EBTNodeResult::Failed);
		
	}
	
	
	
}

EBTNodeResult::Type UGBTTask_Reload::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	bFunctionRunning = false;
    GetWorld()->GetTimerManager().ClearTimer(AbortFunctionTimerHandle);
	
	return EBTNodeResult::Succeeded;
}
