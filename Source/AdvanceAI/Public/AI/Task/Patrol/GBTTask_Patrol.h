// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Navigation/PathFollowingComponent.h"

#include "GBTTask_Patrol.generated.h"

/**
 * 
 */
class AGCharacter;
struct FBTPatrolTask
{
	UBehaviorTreeComponent* BTComp;
	
	AAIController* AIC;
	
	AGCharacter* CharacterPawn;
	
	FDelegateHandle PatrolDelegateHandle;
	
	
	
};

UCLASS()
class ADVANCEAI_API UGBTTask_Patrol : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float PatrolSpeed = 300.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float AcceptanceRadius = 50.f;
	
	bool bBindOnce = false;
	
	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	
    
	
	//functions
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	

	void AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp);
	
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	uint16 GetInstanceMemorySize() const override;

	

	
	
};
