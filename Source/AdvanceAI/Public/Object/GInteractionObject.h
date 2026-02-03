// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagAssetInterface.h"
#include "GameFramework/Actor.h"
#include "GInteractionObject.generated.h"

UCLASS()
class ADVANCEAI_API AGInteractionObject : public AActor, public  IGameplayTagAssetInterface
{
	GENERATED_BODY()
	//variables
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FGameplayTagContainer ActiveGameplayTagContainer;
	//functions
public:	
	// Sets default values for this actor's properties
	AGInteractionObject();
	
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

	
	virtual void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;
	
	virtual bool HasMatchingGameplayTag(FGameplayTag TagToCheck) const;
	
	bool HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const;
	
	bool HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const;


	

};
