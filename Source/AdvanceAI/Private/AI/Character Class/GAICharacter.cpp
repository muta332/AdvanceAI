// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Character Class/GAICharacter.h"

#include "AIController.h"
#include "BrainComponent.h"
#include "AdvanceAI/Macros.h"
#include "AdvanceAI/Public/Widget/EnemyHealthWidget/GEnemyPopUpWidget.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Systems/HealhtAndDamageSystem/GHealthAndDamageSystem.h"


void AGAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CVarEnemyTagContainer.GetValueOnGameThread())
	{
		int32 Key = GetUniqueID();
		FString DebugMas = GetName()+ " = " + ActiveGameplayTagContainer.ToString();
		PRINT(Key,0.f, FColor::Green,  DebugMas);
	}
}

void AGAICharacter::GetOwnedGameplayTags(FGameplayTagContainer& TagContainer) const
{
	TagContainer = ActiveGameplayTagContainer;
}

void AGAICharacter::OnDamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageCauser, EDamageType DamageType)
{
	Super::OnDamageResponse_Implementation(DamageResponse, DamageCauser, DamageType);

	HealthWidgetOnDamageResponse(DamageType, DamageCauser);
	
	if (AttachedPlayerWidget)
	{
		AttachedPlayerWidget->SetPercent(HealthAndDamageSystem->Health, HealthAndDamageSystem->MaxHealth);
	}
	
}

void AGAICharacter::HealthWidgetOnDamageResponse(EDamageType DamageType, AActor* DamageCauser)
{
	if (!AttachedPlayerWidget)
	{
		
		if (!ensure(EnemyPopUpHealthWidget)) return;
		//CreateWidget<UGEnemyPopUpWidget>(GetWorld(), EnemyPopUpHealthWidget);
		AttachedPlayerWidget =  CreateWidget<UGEnemyPopUpWidget>(UGameplayStatics::GetPlayerController(this, 0), EnemyPopUpHealthWidget);
		if (!AttachedPlayerWidget) return;
		AttachedPlayerWidget->AddHeight = EnemyPopUpWidgetHeight;
		AttachedPlayerWidget->PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		 
		
	
		AActor* PlayerCharacter =  UGameplayStatics::GetPlayerController(this, 0)->GetPawn();
	
		
	if (ensure(DamageCauser) && PlayerCharacter)
	{
		AttachedPlayerWidget->isHostile = IGCharacterInterface::Execute_CheckifHostile(PlayerCharacter, this);
	}
		
		AttachedPlayerWidget->AttachedActor = this;
		AttachedPlayerWidget->AddToViewport();
	}
}

void AGAICharacter::OnDeath_Implementation()
{
	if (!ensure(AttachedPlayerWidget)) return;
	Super::OnDeath_Implementation();
	AttachedPlayerWidget->RemoveFromParent();
	AAIController* AIC = UAIBlueprintHelperLibrary::GetAIController(this);
	AIC->GetBrainComponent()->StopLogic("Died");
}

void AGAICharacter::UpdatePercent_Implementation()
{
	if (!AttachedPlayerWidget) return;
	AttachedPlayerWidget->SetPercent(HealthAndDamageSystem->Health, HealthAndDamageSystem->MaxHealth);
}




