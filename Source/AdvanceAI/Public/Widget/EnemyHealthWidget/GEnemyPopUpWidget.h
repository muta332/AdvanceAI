// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GEnemyPopUpWidget.generated.h"

class UProgressBar;
/**
 * 
 */
UCLASS(BLueprintable)
class ADVANCEAI_API UGEnemyPopUpWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	AActor* AttachedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	APlayerController* PlayerController;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	float AddHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = true))
	bool isHostile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor RedColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor GreenColor;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;
	
	//functions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetPercent(const float& Health, const float& MaxHealth);
	
	
};
