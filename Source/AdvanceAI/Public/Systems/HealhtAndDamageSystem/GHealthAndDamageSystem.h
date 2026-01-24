// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "Components/ActorComponent.h"
#include "GHealthAndDamageSystem.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnDamageResponse, EDamageResponse, DamageResponse, AActor* , DamageCauser, EDamageType, DamageType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnBlocked,  AActor* , DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnParried,  AActor* , DamageCauser);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnLowOnHealth,  AActor* , DamageCauser, float, Health);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealed,  float, Health, float, MaxHealth);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ADVANCEAI_API UGHealthAndDamageSystem : public UActorComponent
{
	GENERATED_BODY()
// Varuable Section
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health;
	
	//if Health reduces to this float. onlowonHealth delegate will execute;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float ExecuteOnLowHealthfloat;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealth;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsInvincible;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsDead;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsInterruptable;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsBlocking;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool IsParrying;

	UPROPERTY(BlueprintAssignable)
	FOnDamageResponse OnDamageResponse;

	UPROPERTY(BlueprintAssignable)
	FOnBlocked OnBlocked;

	UPROPERTY(BlueprintAssignable)
	FOnDeath OnDeath;

	UPROPERTY(BlueprintAssignable)
	FOnParried OnParried;
	
	UPROPERTY(BlueprintAssignable)
	FOnLowOnHealth OnLowOnHealth;
	
	UPROPERTY(BlueprintAssignable)
	FOnHealed OnHealed;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AActor* DamageCauser;

public:	
	// Sets default values for this component's properties
	UGHealthAndDamageSystem();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TakeDamage(const FDamageInfo& DamageInfo, bool& WasDamaged);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	float Heal(const float& healAmount);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	

	
	
	void DoDamage(const FDamageInfo& DamageInfo);

	

	void NoDamage(bool WasParrySuccessful, const FDamageInfo& DamageInfo);

	

		
};
