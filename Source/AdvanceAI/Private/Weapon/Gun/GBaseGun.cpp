// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/Gun/GBaseGun.h"

#include "NaniteSceneProxy.h"
#include "AdvanceAI/Macros.h"
#include "Components/SkeletalMeshComponent.h"
#include "AdvanceAI/Public/Systems/GunSystem/GGunSystem.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "InterFaces/GCharacterInterface.h"
#include "AdvanceAI/Macros.h"
#include "AdvanceAI/Public/InterFaces/GAIInterface.h"


// Sets default values
AGBaseGun::AGBaseGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(SceneComponent);
	
	
	

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>("GunSkeletalMesh");
	SkeletalMeshComp->SetupAttachment(SceneComponent);
	SkeletalMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECR_Ignore);

	GunBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	GunBoxComp->SetupAttachment(SkeletalMeshComp);

	GunSystem = CreateDefaultSubobject<UGGunSystem>(TEXT("GunSystem"));

    SkeletalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));

	

}

// Called when the game starts or when spawned
void AGBaseGun::BeginPlay()
{
	Super::BeginPlay();

	
    

	 if (SkeletalMeshComp && GunSystem)
	 {
	 	GunSystem->GunSkeletalMesh = SkeletalMeshComp;
	 }

	if (bIsAttachedToCharacter && ensure(GunBoxComp &&  SkeletalMeshComp))
	{
		SkeletalMeshComp->SetSimulatePhysics(false);
		
	}
	else if (ensure(GunBoxComp &&  SkeletalMeshComp))
	{
		
		SkeletalMeshComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
		SkeletalMeshComp->SetCollisionProfileName(TEXT("PhysicsActor"));
		SkeletalMeshComp->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);

		SkeletalMeshComp->SetSimulatePhysics(true);
		GunBoxComp->SetCollisionEnabled(ECollisionEnabled::Type::QueryOnly);
		GunBoxComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
		GunBoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
		GunBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		GunBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
		GunBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_PhysicsBody, ECollisionResponse::ECR_Ignore);
		GunBoxComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Ignore);
	}
	


	
}

void AGBaseGun::GetInteractText_Implementation(FText& Text, FText& RemainingAmmo)
{
	IGInteractionInterface::GetInteractText_Implementation(Text, RemainingAmmo);
	Text = InteractionText;

	
	
}

void AGBaseGun::Interact_Implementation(AActor* PlayerInstigator)
{

	
	
	if (ensure(SkeletalMeshComp) && ensure(GunBoxComp))
	{
		SkeletalMeshComp->SetCollisionProfileName(TEXT("NoCollision"));
		GunBoxComp->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		
		IGCharacterInterface::Execute_EquipWeapon(PlayerInstigator ,this->GetClass());
		if (!bIsAttachedToCharacter)
		{
			GetWorldTimerManager().SetTimerForNextTick([this]()
		    {
			Destroy();
		    });
		}
		
	}
}



// Called every frame
void AGBaseGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (CVarGunTagContainer.GetValueOnGameThread())
	{
		int32 Key = GetUniqueID();
		FString DebugMas = GetName()+ " = " + ActiveGameplayTagContainer.ToString();
		PRINT(Key,0.f, FColor::Green,  DebugMas)
	}
	
	

}

void AGBaseGun::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Tags.AddUnique(GunTag.GetTagName());
	IGamePlayTagInterFaces::Execute_AddTag(this,GunTag);
	
	
}

void AGBaseGun::AddTag_Implementation(const FGameplayTag AddTag)
{
	
	
		ActiveGameplayTagContainer.AddTag(AddTag);
	
	
		
	
}

void AGBaseGun::RemoveTag_Implementation(const FGameplayTag RemoveTag)
{
	IGamePlayTagInterFaces::RemoveTag_Implementation(RemoveTag);
	ActiveGameplayTagContainer.RemoveTag(RemoveTag);
}

FGameplayTagContainer AGBaseGun::GetActiveGamePlayTagContainer_Implementation()
{
	return ActiveGameplayTagContainer;
}




