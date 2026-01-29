// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/General/BTTask_FindCover.h"
#include "AdvanceAI/Public/InterFaces/GCharacterInterface.h"
#include "AIController.h"
#include "AdvanceAI/Public/Player/GCharacter.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "AdvanceAI/Macros.h"

EBTNodeResult::Type UBTTask_FindCover::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AIC = OwnerComp.GetAIOwner();
	
	AGCharacter* Pawn = Cast<AGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!(AIC && Pawn && FindCoverEnvQuery))
	{
		PRINT(1, 5.f, FColor::Red, TEXT("AIC  or Pawn or FindCover EQS is invalid in BTTask_FindCover"))
		return EBTNodeResult::Failed;
	}
	
	FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	if (!FindCoverMemory)
	{
		PRINTONE("Find Cover Task Memory is Invalid")
		return EBTNodeResult::Failed;
	}
	FindCoverMemory->AIC = AIC;
	FindCoverMemory->Pawn = Pawn;
	FindCoverMemory->BTComp = &OwnerComp;
	
	UPathFollowingComponent* PFC = AIC->GetPathFollowingComponent();
	if (PFC)
	{
		FindCoverMemory->PFCDelegateHandle = PFC->OnRequestFinished.AddUObject(this, &UBTTask_FindCover::AIMoveCompleted, &OwnerComp);
	}
	
	
	IGCharacterInterface::Execute_SetSpeed(Pawn, Speed);
	
		
	FEnvQueryRequest FindCoverEQSRequest(FindCoverEnvQuery, this);
		
	int32 QueryID = FindCoverEQSRequest.Execute(EEnvQueryRunMode::RandomBest25Pct, this, &UBTTask_FindCover::OnFindCoverEQSCompleted);
		
	FindCoverMemory->QueryID = QueryID;
		
	NodeMemoryContainer.Add(QueryID, NodeMemory);
	
	
	
	
	
	return EBTNodeResult::InProgress;
	
}

void UBTTask_FindCover::OnFindCoverEQSCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	
	
	uint8** NodeMemory = NodeMemoryContainer.Find(Result->QueryID);
	if (NodeMemory && *NodeMemory)
	{
		FBTFindCover* FindCoverMemory  = reinterpret_cast<FBTFindCover*>(*NodeMemory);
		
		if (FindCoverMemory)
		{
			if (Result->IsSuccessful())
			{
				FindCoverMemory->AIC->MoveToLocation(Result->GetItemAsLocation(0), AcceptanceRadius);
				
			}
			else if (!Result->IsSuccessful())
			{
				PRINTONE("FindCover EQS Failed")
				FinishLatentTask(*FindCoverMemory->BTComp, EBTNodeResult::Failed);
			}
		}
	}
	else
	{
		PRINTONE("NodeMemory Invalid")
		
	}

}

void UBTTask_FindCover::AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp)
{
	uint8* NodeMemory  = OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this));
	
	
	
	
	
		 FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	
	
	if (FindCoverMemory)
	{
		if (Result.IsSuccess())
		{
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
		else if (Result.IsFailure())
		{
			FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		}
	}
	
}

uint16 UBTTask_FindCover::GetInstanceMemorySize() const
{
	return sizeof(FBTFindCover);
}

EBTNodeResult::Type UBTTask_FindCover::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UnbindAllDelegate(NodeMemory);
	
	return Super::AbortTask(OwnerComp, NodeMemory);
}

void UBTTask_FindCover::UnbindAllDelegate(uint8* NodeMemory)
{
	FBTFindCover* FindCoverMemory = reinterpret_cast<FBTFindCover*>(NodeMemory);
	
	if (FindCoverMemory && FindCoverMemory->AIC->GetPathFollowingComponent())
	{
		FindCoverMemory->AIC->GetPathFollowingComponent()->OnRequestFinished.Remove(FindCoverMemory->PFCDelegateHandle);
		
		if (FindCoverMemory->QueryID != INDEX_NONE)
		{
			UEnvQueryManager* QueryManager = UEnvQueryManager::GetCurrent(GetWorld());
			
			if (QueryManager)
			{
				QueryManager->AbortQuery(FindCoverMemory->QueryID);
				NodeMemoryContainer.Remove(FindCoverMemory->QueryID);
			}
			FindCoverMemory->QueryID = INDEX_NONE;
		}
		
	}
}

void UBTTask_FindCover::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                       EBTNodeResult::Type TaskResult)
{
	UnbindAllDelegate(NodeMemory);
	
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}


	
	

