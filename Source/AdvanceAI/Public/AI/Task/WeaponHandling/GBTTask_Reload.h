// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Weapon/Gun/GBaseGun.h"

#include "GBTTask_Reload.generated.h"

/**
 * 
 */
class UGGunSystem;

class AGCharacter;

class UProxyObject;

struct FBTReload
{
	UBehaviorTreeComponent* BTComp;
	
	AGCharacter* Pawn;
	
	UGGunSystem* GunSystem;
	
	UBTTaskNode* NodeTask;
	
	AAIController* AIC;
	
	UProxyObject* Proxy;
};


UCLASS(Blueprintable)
class ADVANCEAI_API UGBTTask_Reload : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	
protected:
	void CleanUpReloadTask(uint8* NodeMemory);
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	EBTNodeResult::Type DoReloadTask(AGCharacter* AIPawn, AActor* WeaponActor, UBehaviorTreeComponent* BTComp);
	void InitializeTaskMemory(UBehaviorTreeComponent& OwnerComp, FBTReload* TaskMemory, AGBaseGun* WeaponActor,
	                          AGCharacter* AI);

	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
	
	FTimerHandle AbortFunctionTimerHandle;
	

	
	void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;
	
	//Writable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	float AbortTimer = 20;
	
	
	
	
};

UCLASS()
class UProxyObject : public UObject
{
	GENERATED_BODY()
public:
	FBTReload ReloadDataTask;
	
	
	void BindFunctions();
	
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);
	
	
};