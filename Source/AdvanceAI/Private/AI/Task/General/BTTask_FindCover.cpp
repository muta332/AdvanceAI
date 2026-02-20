// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/General/BTTask_FindCover.h"
#include "AdvanceAI/Public/InterFaces/GCharacterInterface.h"
#include "AIController.h"
#include "AdvanceAI/Public/Player/GCharacter.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "AdvanceAI/Macros.h"
#include "VisualLogger/VisualLogger.h"

DEFINE_LOG_CATEGORY_STATIC(FindCoverEQSLocation, Log, All)

void UBTTask_FindCover::InitializeFindCoverMemory(UBehaviorTreeComponent& OwnerComp, FBTFindCover* FindCoverMemory, AAIController* AIC, AGCharacter* Pawn, UEnvQueryInstanceBlueprintWrapper* FindCover)
{
	FindCoverMemory->AIC = AIC;
	FindCoverMemory->Pawn = Pawn;
	FindCoverMemory->BTComp = &OwnerComp;
	FindCoverMemory->MainNode = this;
	FindCoverMemory->AcceptanceRadius = AcceptanceRadius;
	FindCoverMemory->EQSFindCover = FindCover;
	
}

EBTNodeResult::Type UBTTask_FindCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	
	if (!FindCoverMemory)
	{
		PRINTONE("Find Cover Task Memory is Invalid")
		return EBTNodeResult::Failed;
	}
	
	AAIController* AIC = OwnerComp.GetAIOwner();
	
	AGCharacter* Pawn = Cast<AGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!(AIC && Pawn && FindCoverEnvQuery))
	{
		PRINT(1, 5.f, FColor::Red, TEXT("AIC  or Pawn or FindCover EQS is invalid in BTTask_FindCover"))
		return EBTNodeResult::Failed;
	}
	
	IGCharacterInterface::Execute_SetSpeed(Pawn, Speed);
	
	

	UEnvQueryInstanceBlueprintWrapper* FindCover = UEnvQueryManager::RunEQSQuery(this, FindCoverEnvQuery,& OwnerComp,EEnvQueryRunMode::RandomBest25Pct, nullptr);	
	if (FindCoverMemory && AIC &&  Pawn && FindCover)
	{
		InitializeFindCoverMemory(OwnerComp, FindCoverMemory, AIC, Pawn, FindCover);
	}
	
	
	
	if (!FindCoverMemory->Proxy.IsValid())
	{
		TWeakObjectPtr<UProxy> Proxy = NewObject<UProxy>(&OwnerComp);
		Proxy->FindCoverMemory = *FindCoverMemory;
		FindCoverMemory->Proxy = Proxy;
		
		
		
	}
	if (IsValid(FindCoverMemory->Proxy.Get()))
	{
		BindFunctions(NodeMemory);
	}
	
	return EBTNodeResult::InProgress;
	
}





uint16 UBTTask_FindCover::GetInstanceMemorySize() const
{
	return sizeof(FBTFindCover);
}

EBTNodeResult::Type UBTTask_FindCover::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	if (FindCoverMemory && FindCoverMemory->Proxy.Get())
	{
		UnbindFunctions(NodeMemory);
	
		
	}
	
	return EBTNodeResult::Aborted;
}

void UBTTask_FindCover::UnbindFunctions(uint8* NodeMemory)
{
	FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	if (FindCoverMemory->AIC && FindCoverMemory->EQSFindCover &&FindCoverMemory)
	{
		
		FindCoverMemory->AIC->ReceiveMoveCompleted.RemoveDynamic(FindCoverMemory->Proxy.Get(), &UProxy::AIMoveCompleted);
		FindCoverMemory->EQSFindCover->GetOnQueryFinishedEvent().RemoveDynamic(FindCoverMemory->Proxy.Get(), &UProxy::OnFindCoverEQSCompleted);
		
	}
}


void UBTTask_FindCover::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       EBTNodeResult::Type TaskResult)
{
	FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	if (FindCoverMemory && FindCoverMemory->Proxy.Get())
	{
		UnbindFunctions(NodeMemory);
		
		
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}

void UBTTask_FindCover::BindFunctions(uint8* NodeMemory)
{
	FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	if (FindCoverMemory->AIC && FindCoverMemory->EQSFindCover && FindCoverMemory->Proxy.Get())
	{
		FScriptDelegate Delegate;
		FindCoverMemory->AIC->ReceiveMoveCompleted.AddDynamic( FindCoverMemory->Proxy.Get(), &UProxy::AIMoveCompleted);
		FindCoverMemory->EQSFindCover->GetOnQueryFinishedEvent().AddDynamic( FindCoverMemory->Proxy.Get(), &UProxy::OnFindCoverEQSCompleted);
	}
}

	
void UProxy::AIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	// CRITICAL: Validate everything first

	FBTFindCover Memory = FindCoverMemory;
   
	if (!IsValid(Memory.BTComp) || !IsValid(Memory.BTComp))
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveCompleted: BTComp is invalid"));
		Memory.MainNode->FinishLatentTask(*Memory.BTComp, EBTNodeResult::Failed);
		return;
	}
    
	if (!Memory.MainNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveCompleted: MainNode is null"));
		Memory.MainNode->FinishLatentTask(*Memory.BTComp, EBTNodeResult::Failed);
		return;
	}
    
	// Cache values before potentially destroying ourselves
	UBehaviorTreeComponent* BTComp = Memory.BTComp;
	UBTTaskNode* MainNode = Memory.MainNode;
    
	uint8* NodeMemory = BTComp->GetNodeMemory(MainNode, BTComp->FindInstanceContainingNode(MainNode));
    
	if (!NodeMemory)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveCompleted: NodeMemory is null"));
		Memory.MainNode->FinishLatentTask(*Memory.BTComp, EBTNodeResult::Failed);
		return;
	}
    
	FBTFindCover* TaskNodeMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
    
	if (!TaskNodeMemory || !TaskNodeMemory->MainNode)
	{
		UE_LOG(LogTemp, Warning, TEXT("AIMoveCompleted: Memory or Memory->MainNode is null"));
		Memory.MainNode->FinishLatentTask(*Memory.BTComp, EBTNodeResult::Failed);
		return;
	}
    
	// Determine result BEFORE calling FinishLatentTask
	EBTNodeResult::Type TaskResult = (Result == EPathFollowingResult::Success) 
		? EBTNodeResult::Succeeded 
		: EBTNodeResult::Failed;
    
	// This may destroy the proxy, so don't access 'this' after this call
	MainNode->FinishLatentTask(*BTComp, TaskResult);
}
/*
void UProxy::AIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result
)
{
	
	
	FBTFindCover* Memory = FindCoverMemory;
	
	
	if (Memory && Memory->MainNode)
	{
		if (Result == EPathFollowingResult::Success)
		{
			Memory->MainNode->FinishLatentTask(*FindCoverMemory->BTComp, EBTNodeResult::Succeeded);
		}
		else if (Result != EPathFollowingResult::Success)
		{
			Memory->MainNode->FinishLatentTask(*FindCoverMemory->BTComp, EBTNodeResult::Failed);
		}
	}
}
*/
/*
void UProxy::OnFindCoverEQSCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
	EEnvQueryStatus::Type QueryStatus)
{ 
	if (QueryStatus == EEnvQueryStatus::Success && FindCoverMemory->MainNode)
			{
				TArray<FVector> Locations;
				QueryInstance->GetQueryResultsAsLocations(Locations);
				EPathFollowingRequestResult::Type Result = FindCoverMemory->AIC->MoveToLocation(Locations[0], FindCoverMemory->AcceptanceRadius);
		
		if (Result == EPathFollowingRequestResult::Failed)
		{
			FindCoverMemory->MainNode->FinishLatentTask(*FindCoverMemory->BTComp, EBTNodeResult::Failed);
		}
		if (Result == EPathFollowingRequestResult::RequestSuccessful)
		{
			PRINTONE("FindCover EQS Success")
		}
		else
		{
			PRINTONE("FindCover EQS FAILED")
			FindCoverMemory->MainNode->FinishLatentTask(*FindCoverMemory->BTComp, EBTNodeResult::Failed);
		}
				
        #if ENABLE_VISUAL_LOG
				FVisualLogger& VLog = FVisualLogger::Get();
				
				if (VLog.IsRecording())
				{
					UE_VLOG_SPHERE(this, FindCoverEQSLocation, Warning, Locations[0], 50.f, FColor::Green, TEXT("Move To Location"));
				}
				
				
        #endif
				
			}
			else if (QueryStatus == EEnvQueryStatus::Failed || QueryStatus == EEnvQueryStatus::Aborted)
			{
				PRINTONE("FindCover EQS Failed")
				FindCoverMemory->MainNode->FinishLatentTask(*FindCoverMemory->BTComp, EBTNodeResult::Failed);
			}
		
	
	
}
*/
void UProxy::OnFindCoverEQSCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
    EEnvQueryStatus::Type QueryStatus)
{
    // CRITICAL: Check everything before accessing anything
  
	
	FBTFindCover LocalFindCoverMemory = FindCoverMemory;
    
    if (!LocalFindCoverMemory.MainNode)
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFindCoverEQSCompleted: MainNode is null"));
    	LocalFindCoverMemory.MainNode->FinishLatentTask(*LocalFindCoverMemory.BTComp, EBTNodeResult::Failed);
        return;
    }
    
    if (!LocalFindCoverMemory.BTComp || !IsValid(LocalFindCoverMemory.BTComp))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFindCoverEQSCompleted: BTComp is invalid"));
    	LocalFindCoverMemory.MainNode->FinishLatentTask(*LocalFindCoverMemory.BTComp, EBTNodeResult::Failed);
        return;
    }
    
    if (!LocalFindCoverMemory.AIC || !IsValid(LocalFindCoverMemory.AIC))
    {
        UE_LOG(LogTemp, Warning, TEXT("OnFindCoverEQSCompleted: AIC is invalid"));
    	LocalFindCoverMemory.MainNode->FinishLatentTask(*LocalFindCoverMemory.BTComp, EBTNodeResult::Failed);
        return;
    }
    
    // Cache values BEFORE any operations that might destroy this proxy
    UBehaviorTreeComponent* BTComp =LocalFindCoverMemory.BTComp;
    UBTTaskNode* MainNode = LocalFindCoverMemory.MainNode;
    AAIController* AIC = LocalFindCoverMemory.AIC;
    float AcceptRadius = LocalFindCoverMemory.AcceptanceRadius;
    
    if (QueryStatus == EEnvQueryStatus::Success)
    {
        TArray<FVector> Locations;
        QueryInstance->GetQueryResultsAsLocations(Locations);
        
        if (Locations.Num() > 0)
        {
            EPathFollowingRequestResult::Type Result = AIC->MoveToLocation(Locations[0], AcceptRadius);
            
            if (Result == EPathFollowingRequestResult::Failed)
            {
                PRINTONE("FindCover EQS Move Request Failed")
                MainNode->FinishLatentTask(*BTComp, EBTNodeResult::Failed);
                return; // Don't access 'this' after FinishLatentTask
            }
            else if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
            {
                PRINTONE("FindCover Already At Goal")
                MainNode->FinishLatentTask(*BTComp, EBTNodeResult::Succeeded);
                return; // Don't access 'this' after FinishLatentTask
            }
            // Otherwise RequestSuccessful - wait for AIMoveCompleted callback
            
            #if ENABLE_VISUAL_LOG
            FVisualLogger& VLog = FVisualLogger::Get();
            if (VLog.IsRecording())
            {
                UE_VLOG_SPHERE(this, FindCoverEQSLocation, Warning, Locations[0], 50.f, FColor::Green, TEXT("Move To Location"));
            }
            #endif
        }
        else
        {
            PRINTONE("FindCover EQS Success but no locations")
            MainNode->FinishLatentTask(*BTComp, EBTNodeResult::Failed);
        }
    }
    else if (QueryStatus == EEnvQueryStatus::Failed || QueryStatus == EEnvQueryStatus::Aborted)
    {
        PRINTONE("FindCover EQS Failed")
        MainNode->FinishLatentTask(*BTComp, EBTNodeResult::Failed);
    }
}

	
	

