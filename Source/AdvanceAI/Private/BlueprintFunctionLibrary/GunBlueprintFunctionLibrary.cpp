// Fill out your copyright notice in the Description page of Project Settings.


#include "BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "AdvanceAI/Public/InterFaces/GamePlayTagInterFaces.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "Player/ABasePlayerCharacter.h"




void UGunBlueprintFunctionLibrary::DeleteTagBatch(AActor* Instigator, FGameplayTag ParentTag)
{
	if (Instigator && Instigator->Implements<UGamePlayTagInterFaces>())
	{
		FGameplayTagContainer TagContainer = IGamePlayTagInterFaces::Execute_GetActiveGamePlayTagContainer(Instigator);
		

		for (const FGameplayTag& Tag : TagContainer.GetGameplayTagArray())
		{
			if (Tag.MatchesTag(ParentTag))
			{
				IGamePlayTagInterFaces::Execute_RemoveTag(Instigator, Tag);
			}
		}
	}
}

void UGunBlueprintFunctionLibrary::CameraAim(
	ACharacter* Instigator,
	float CameraTraceLength,
	const FName SpawnLocationBoneName,


	FVector& EndLocation,
	FVector& SpawnStart,
	FVector& StartLocation,
	FRotator& SpawnRotation)
{
	if (Instigator)
	{
		ABasePlayerCharacter* Player = Cast<ABasePlayerCharacter>(Instigator);

		if (Instigator && Player)
		{
			StartLocation = Player->GetFollowCamera()->GetComponentLocation();

			FVector CameraRotationForwardVector = Player->GetFollowCamera()->GetComponentRotation().Vector();

			EndLocation =  StartLocation + (CameraRotationForwardVector * CameraTraceLength);

			SpawnStart = Player->GetMesh()->GetSocketLocation(SpawnLocationBoneName);

			SpawnRotation = UKismetMathLibrary::FindLookAtRotation(SpawnStart, EndLocation);
		}
        
	}
	
	
	
	
}

void UGunBlueprintFunctionLibrary::GetDistanceBetweenTwoActors(AActor* ActorOne, AActor* ActorTwo, float& Distance)
{
	if (!(ActorOne && ActorTwo)) return;
	FVector ActorOneLocation = ActorOne->GetActorLocation();
	FVector ActorTwoLocation = ActorTwo->GetActorLocation();
	Distance = FVector::Dist(ActorOneLocation, ActorTwoLocation);
}

bool UGunBlueprintFunctionLibrary::DoesActorImplementInterFace(AActor* Actor, TSubclassOf<UInterface> InterfaceClass)
{
	return Actor->GetClass()->ImplementsInterface(InterfaceClass);
}



	



