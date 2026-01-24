// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player/GCharacter.h"
#include "GameplayTagAssetInterface.h"

#include "GAICharacter.generated.h"


class UGEnemyPopUpWidget;
/**
 * 
 */
static  TAutoConsoleVariable<bool> CVarEnemyTagContainer (TEXT("su.EnemyTagContainer"), false,TEXT("if true, Shows Tags of all the spawned guns tag"));
UCLASS(Blueprintable)
class ADVANCEAI_API AGAICharacter : public AGCharacter, public IGameplayTagAssetInterface
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UGEnemyPopUpWidget* AttachedPlayerWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UGEnemyPopUpWidget> EnemyPopUpHealthWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemyPopUpWidgetHeight;
protected:
	 void Tick(float DeltaTime) override;

	 void GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const override;

	
	 void OnDamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageCauser, EDamageType DamageType) override;

	void HealthWidgetOnDamageResponse(EDamageType DamageType, AActor* DamageCauser);

	void OnDeath_Implementation() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdatePercent();
};
