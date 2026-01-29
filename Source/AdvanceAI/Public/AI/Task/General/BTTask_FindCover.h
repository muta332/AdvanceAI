// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_FindCover.generated.h"

/**
 * 
 */
class UEnvQuery;
class AGCharacter;
class UEnvQueryInstanceBlueprintWrapper;
struct FBTFindCover
{
	UBehaviorTreeComponent* BTComp;
	
	
	AAIController* AIC;
	
	
	AGCharacter* Pawn;
	
	FDelegateHandle PFCDelegateHandle;
	
	FDelegateHandle CoverDelegateHandle;
	
	int32 QueryID;
};

UCLASS()
class ADVANCEAI_API UBTTask_FindCover : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float Speed = 800.f;
protected: 
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//UPROPERTY()
	//UEnvQueryInstanceBlueprintWrapper* FindCoverEQS;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float AcceptanceRadius = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	UEnvQuery* FindCoverEnvQuery;
	
	
	void OnFindCoverEQSCompleted(TSharedPtr<FEnvQueryResult> Result);
	
	
	static TMap<int32, uint8*> NodeMemoryContainer;
	
	void AIMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result, UBehaviorTreeComponent* OwnerComp);
	
	uint16 GetInstanceMemorySize() const override;
	
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void UnbindAllDelegate(uint8* NodeMemory);

	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult);
};
