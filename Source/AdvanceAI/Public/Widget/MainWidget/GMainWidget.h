// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GMainWidget.generated.h"

/**
 * 
 */
class UVerticalBox;
class UHorizontalBox;
class UTextBlock;
class USpacer;
class UProgressBar;
class UGCrosshair;
UCLASS(Blueprintable)
class ADVANCEAI_API UGMainWidget : public UUserWidget
{
	GENERATED_BODY()
public: 
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UVerticalBox* UtilityBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UProgressBar* HealthBar;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UHorizontalBox* AmmoCount;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UTextBlock* RemainingAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UTextBlock* DividerText;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UTextBlock* StorageAmmo;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	USpacer* Spacer;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (BindWidget))
	UGCrosshair* WBP_Crosshair;
	
	//functions
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetHealthBar(const float& InPercent);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetStorageAmmo(const FText& AmountText);
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetRemainingAmmo(const FText& Amount);

	
	
	
};
