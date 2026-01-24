// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GInteractionWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UVerticalBox;
UCLASS(Blueprintable)
class ADVANCEAI_API UGInteractionWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	//Widget Variables
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UVerticalBox* VerticalBox;
	
	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* InteractionTextVariable;

	UPROPERTY(meta = (BindWidget), BlueprintReadOnly)
	UTextBlock* RemainingAmmoText;
	//Class Variables
	UPROPERTY(BlueprintReadOnly ,meta = (ExposeOnSpawn= true))
	FText InteractionText;

	UPROPERTY(BlueprintReadOnly,meta = (ExposeOnSpawn= true))
	AActor* AttachActor;

	UPROPERTY(BlueprintReadOnly,meta = (ExposeOnSpawn= true))
	FText RemainingAmmo;

	//Function
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetLocationOfText();

	
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SetRemainingAmmoText(const FText& InText);
};
 