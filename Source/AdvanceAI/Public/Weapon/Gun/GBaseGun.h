// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AdvanceAI/Public/InterFaces/GInteractionInterface.h"
#include "GameplayTagContainer.h"
#include "InterFaces/GamePlayTagInterFaces.h"
#include "GBaseGun.generated.h"

class USceneComponent;
class UBoxComponent;
class UGGunSystem;
class USkeletalMeshComponent;
static  TAutoConsoleVariable<bool> CVarGunTagContainer (TEXT("su.GunTagContainer"), false,TEXT("if true, Shows Tags of all the spawned guns tag"));
UCLASS(Blueprintable)
class ADVANCEAI_API AGBaseGun : public AActor, public IGInteractionInterface, public IGamePlayTagInterFaces
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGBaseGun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void GetInteractText_Implementation(FText& Text, FText& RemainingAmmo) override;

	void Interact_Implementation(AActor* PlayerInstigator);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* SceneComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* SkeletalMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UGGunSystem* GunSystem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBoxComponent* GunBoxComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText InteractionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	bool bIsAttachedToCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	FGameplayTagContainer ActiveGameplayTagContainer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn))
	FGameplayTag GunTag;

	void OnConstruction(const FTransform& Transform) override;

	void AddTag_Implementation(const FGameplayTag AddTag ) override;

	void RemoveTag_Implementation	(const FGameplayTag RemoveTag ) override;

	FGameplayTagContainer GetActiveGamePlayTagContainer_Implementation() override;

	

	


	

	


	
	

	
};
