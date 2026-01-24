// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GCrosshair.generated.h"

/**
 * 
 */
class UImage;
UCLASS(Blueprintable)
class ADVANCEAI_API UGCrosshair : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget))
	UImage* Crosshair;

	
};
