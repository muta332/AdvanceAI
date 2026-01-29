// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Task/Patrol/GBTTask_Patrol.h"
#include "AdvanceAI/Public/InterFaces/GAIInterface.h"
#include "InterFaces/GCharacterInterface.h"
#include "AdvanceAI/Public/Player/GCharacter.h"
#include "AIController.h"
#include "AdvanceAI/Macros.h"


void UGBTTask_Patrol::OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
                                     EBTNodeResult::Type TaskResult)
{
	FBTPatrolTask* PatrolTaskStuct = reinterpret_cast<FBTPatrolTask*>(NodeMemory);
	
	if (PatrolTaskStuct)
	{
		PatrolTaskStuct->AIC->GetPathFollowingComponent()->OnRequestFinished.Remove(PatrolTaskStuct->PatrolDelegateHandle);
		PatrolTaskStuct->PatrolDelegateHandle.Reset();
	}
	Super::OnTaskFinished(OwnerComp, NodeMemory, TaskResult);
}



EBTNodeResult::Type UGBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	 AAIController* AIC = OwnerComp.GetAIOwner();
	AGCharacter* Pawn = Cast<AGCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	
	if (!(AIC && Pawn)) return EBTNodeResult::Failed;
	
	FBTPatrolTask* PatrolTaskStuct = reinterpret_cast<FBTPatrolTask*>(NodeMemory);
	if (!PatrolTaskStuct)
	{
		PRINTONE("NodeMemory is Invalid")
		return EBTNodeResult::Failed;
	}
	PatrolTaskStuct->AIC = AIC;
	PatrolTaskStuct->CharacterPawn = Pawn;
	PatrolTaskStuct->BTComp = &OwnerComp;
	
	UPathFollowingComponent* PFC = AIC->GetPathFollowingComponent();
	if (PFC && !PatrolTaskStuct->PatrolDelegateHandle.IsValid())
	{
		PatrolTaskStuct->PatrolDelegateHandle = PFC->OnRequestFinished.AddUObject(this, &UGBTTask_Patrol::AIMoveCompleted, &OwnerComp);
	}
	
	FVector PatrolLocation;

	PatrolLocation = IGAIInterface::Execute_GetSplineLocation(OwnerComp.GetAIOwner()->GetPawn());
	 
	
	IGCharacterInterface::Execute_SetSpeed(OwnerComp.GetAIOwner()->GetPawn(), PatrolSpeed);

	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(PatrolLocation);
	MoveRequest.SetAcceptanceRadius(AcceptanceRadius);
	
	AIC->MoveTo(MoveRequest);
	

	return EBTNodeResult::InProgress;
}

void UGBTTask_Patrol::AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp)
{
	
	if (!OwnerComp)
	{
		PRINTONE("Node Memory invalid Check Patrol Task")
		return;
	}
	
	
	uint8* NodeMemory = OwnerComp->GetNodeMemory(this, OwnerComp->FindInstanceContainingNode(this));
	
	FBTPatrolTask* PatrolTask = reinterpret_cast<FBTPatrolTask*>(NodeMemory);
	if (PatrolTask)
	{
		if (Result.IsSuccess())
		{
			UBehaviorTreeComponent* OwnerComp = PatrolTask->BTComp;
			FinishLatentTask(*OwnerComp, EBTNodeResult::Succeeded);
		}
		else if (Result.IsFailure())
		{
			UBehaviorTreeComponent* OwnerComp = PatrolTask->BTComp;
			FinishLatentTask(*OwnerComp, EBTNodeResult::Failed);
		}
		else
		{
			PRINTONE("AI Movement Stoped but Result is Unkonwn")
		}
		
		
		PatrolTask->AIC->GetPathFollowingComponent()->OnRequestFinished.Remove(PatrolTask->PatrolDelegateHandle);
		
		PatrolTask->PatrolDelegateHandle.Reset();
	}
	else
	{
		PRINTONE("AI Move Completed but Nodememory is Invalid")
	}

	bBindOnce = false;

	
	
	
	
	
	
}

EBTNodeResult::Type UGBTTask_Patrol::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	FBTPatrolTask* PatrolTask = reinterpret_cast<FBTPatrolTask*>(NodeMemory);
	if (PatrolTask)
	{
		PatrolTask->AIC->StopMovement();
		
		PatrolTask->AIC->GetPathFollowingComponent()->OnRequestFinished.Remove(PatrolTask->PatrolDelegateHandle);
		PatrolTask->PatrolDelegateHandle.Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Aborted);
	}
	
	
	
	
	return EBTNodeResult::Aborted;
}

uint16 UGBTTask_Patrol::GetInstanceMemorySize() const
{
	return sizeof(FBTPatrolTask);
}
