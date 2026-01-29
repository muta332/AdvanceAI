// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Player/GCharacter.h"
#include "GBTTask_Reload.generated.h"

/**
 * 
 */
class UGGunSystem;
UCLASS(Blueprintable)
class ADVANCEAI_API UGBTTask_Reload : public UBTTaskNode
{
	GENERATED_BODY()
public: 
	UFUNCTION()
	void MontageEnded(UAnimMontage* Montage, bool bInterrupted);
protected:
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void DoReloadTask(AGCharacter* AI);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	bool bFunctionRunning = false;
	
	UGGunSystem* GunSystem;
	
	FTimerHandle AbortFunctionTimerHandle;
	
	AGCharacter* Character;
	
	UBehaviorTreeComponent* OwnerComponent;
	
	void AbortFunctionIfFailedToReload();
	
	//Writable
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	float AbortTimer = 20;
	
	
	
	
};
