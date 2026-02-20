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
class UProxy;
struct FBTFindCover;

USTRUCT()
struct FBTFindCover 
{

  GENERATED_BODY()
	UPROPERTY()
 
	TObjectPtr<UBehaviorTreeComponent> BTComp = nullptr;
	
	UPROPERTY()
	TObjectPtr<AAIController> AIC = nullptr;
	
	UPROPERTY()
	TObjectPtr<AGCharacter> Pawn = nullptr;
	
	UPROPERTY()
	TObjectPtr<UEnvQueryInstanceBlueprintWrapper> EQSFindCover = nullptr;
	
	UPROPERTY()
	TObjectPtr<UBTTaskNode> MainNode = nullptr;
	
	UPROPERTY()
	TWeakObjectPtr<UProxy> Proxy = nullptr;
	
	
	TWeakObjectPtr<FBTFindCover>* FindCoverMemory;
	
	float AcceptanceRadius;
	
	
};

UCLASS()
class ADVANCEAI_API UBTTask_FindCover : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float Speed = 800.f;
protected:
	void InitializeFindCoverMemory(UBehaviorTreeComponent& OwnerComp, FBTFindCover* FindCoverMemory, AAIController* AIC,
	                               AGCharacter* Pawn, UEnvQueryInstanceBlueprintWrapper* FindCover);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	//UPROPERTY()
	//UEnvQueryInstanceBlueprintWrapper* FindCoverEQS;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float AcceptanceRadius = 50.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta = (ExposeOnSpawn = true))
	UEnvQuery* FindCoverEnvQuery;
	
	
	uint16 GetInstanceMemorySize() const override;
	
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	void UnbindFunctions(uint8* NodeMemory);

	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	void BindFunctions(uint8* NodeMemory);
};

UCLASS()
class UProxy : public UObject
{
	GENERATED_BODY()
public : 
	
    UPROPERTY()
	FBTFindCover FindCoverMemory;
	
	
	

	
	FDelegateHandle PathFollowingHandle;
	
	UFUNCTION()
	void AIMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result);
	
	UFUNCTION()
	void OnFindCoverEQSCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
};