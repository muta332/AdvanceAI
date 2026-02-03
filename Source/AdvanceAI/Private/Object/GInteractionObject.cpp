// Fill out your copyright notice in the Description page of Project Settings.


#include "Object/GInteractionObject.h"

#include "BehaviorTree/BehaviorTreeTypes.h"

// Sets default values
AGInteractionObject::AGInteractionObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGInteractionObject::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGInteractionObject::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = ActiveGameplayTagContainer;
}

bool AGInteractionObject::HasMatchingGameplayTag(FGameplayTag TagToCheck) const
{
	return ActiveGameplayTagContainer.HasTagExact(TagToCheck);
}

bool AGInteractionObject::HasAllMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return ActiveGameplayTagContainer.HasAll(TagContainer);
}

bool AGInteractionObject::HasAnyMatchingGameplayTags(const FGameplayTagContainer& TagContainer) const
{
	return ActiveGameplayTagContainer.HasAny(TagContainer);
}


// Called every frame
void AGInteractionObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

