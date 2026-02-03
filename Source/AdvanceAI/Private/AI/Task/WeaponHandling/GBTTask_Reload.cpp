// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/WeaponHandling/GBTTask_Reload.h"

#include "AIController.h"

#include "AdvanceAI/Public/Systems/GunSystem/GGunSystem.h"
#include "AdvanceAI/Public/InterFaces/GCharacterInterface.h"
#include "Player/GCharacter.h"








EBTNodeResult::Type UGBTTask_Reload::DoReloadTask(AGCharacter* AIPawn, AActor* WeaponActor,
                                                  UBehaviorTreeComponent* BTComp)
{
	
	
	if (AIPawn && WeaponActor)
	{
		UGGunSystem* GunSystem = WeaponActor->FindComponentByClass<UGGunSystem>();
		if (GunSystem)
		{
			GunSystem->TriggerReleased();
			
			if (GunSystem->CurrentStorage > 0)
			{
				GunSystem->Reloading();
				
				
			}
			else if (GunSystem->CurrentStorage == 0 || GunSystem->CurrentStorage < 0)
			{
				IGamePlayTagInterFaces::Execute_AddTag(AIPawn, FGameplayTag::RequestGameplayTag("Gun.StorageEmty"));
				
				return EBTNodeResult::Failed;
			}
		}
		else
		{
			return EBTNodeResult::Failed;
		}
	}
	
	return EBTNodeResult::InProgress;
}

void UGBTTask_Reload::InitializeTaskMemory(UBehaviorTreeComponent& OwnerComp, FBTReload* TaskMemory, AGBaseGun* WeaponActor, AGCharacter* AI)
{
	TaskMemory->BTComp = &OwnerComp;
	
	TaskMemory->NodeTask = this;
	
	TaskMemory->Pawn = AI;
	
	TaskMemory->AIC = OwnerComp.GetAIOwner();
	
	TaskMemory->GunSystem = WeaponActor->FindComponentByClass<UGGunSystem>();
}

EBTNodeResult::Type UGBTTask_Reload::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!(OwnerComp.GetAIOwner() && OwnerComp.GetAIOwner()->GetPawn())) return EBTNodeResult::Failed;
	
	
	
	
	FBTReload* TaskMemory = reinterpret_cast<FBTReload*>(NodeMemory); 
	
	AGBaseGun* WeaponActor = nullptr;
	
	AGCharacter* AI= Cast<AGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	IGCharacterInterface::Execute_GetWeapon(AI, WeaponActor);
	
	EBTNodeResult::Type NodeResult = EBTNodeResult::Failed;
	
	if (TaskMemory && WeaponActor && AI)
	{
		InitializeTaskMemory(OwnerComp, TaskMemory, WeaponActor, AI);

	NodeResult = DoReloadTask(AI, WeaponActor, &OwnerComp);
 	
 	
	}
		
		
	if (TaskMemory &&!TaskMemory->Proxy && NodeResult == EBTNodeResult::InProgress)
	{
		UProxyObject* Proxy = NewObject<UProxyObject>(&OwnerComp);
		
		TaskMemory->Proxy = Proxy;
		
		TaskMemory->Proxy->ReloadDataTask = *TaskMemory;
		
		
		Proxy->BindFunctions();
		
		
	}
	
	return NodeResult;
	
	
	

	

	
	
	
	
	
}




void UGBTTask_Reload::CleanUpReloadTask(uint8* NodeMemory)
{
	
	
	FBTReload* ReloadData = reinterpret_cast<FBTReload*>(NodeMemory);
	
	if (ReloadData && ReloadData->Pawn && ReloadData->Pawn->AnimInstance && ReloadData->Proxy)
	{
		ReloadData->Pawn->AnimInstance->OnMontageEnded.RemoveDynamic(ReloadData->Proxy, &UProxyObject::MontageEnded);
	}
	
	
}

EBTNodeResult::Type UGBTTask_Reload::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
     CleanUpReloadTask(NodeMemory);
	
	return EBTNodeResult::Aborted;
}

void UGBTTask_Reload::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                     EBTNodeResult::Type TaskResult)
{
	CleanUpReloadTask(NodeMemory);
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UProxyObject::BindFunctions()
{
	if (ReloadDataTask.Pawn && ReloadDataTask.Pawn->AnimInstance)
	{
		ReloadDataTask.Pawn->AnimInstance->OnMontageEnded.AddDynamic(this, &UProxyObject::MontageEnded);
	}
	
}

void UProxyObject::MontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	
	
	if (!(ReloadDataTask.NodeTask && ReloadDataTask.BTComp && ReloadDataTask.Pawn)) return;
	
	
	
	if (!bInterrupted)
	{
		ReloadDataTask.NodeTask->FinishLatentTask(*ReloadDataTask.BTComp, EBTNodeResult::Succeeded);
	}
	else
	{   
		ReloadDataTask.NodeTask->FinishLatentTask(*ReloadDataTask.BTComp, EBTNodeResult::Failed);
		
	}
}
