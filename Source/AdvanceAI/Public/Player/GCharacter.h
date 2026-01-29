// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AdvanceAI/Public/BlueprintFunctionLibrary/GunBlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "InterFaces/GCharacterInterface.h"
#include "InterFaces/GamePlayTagInterFaces.h"
#include "GCharacter.generated.h"
class AGBaseGun;
class UGHealthAndDamageSystem;
UCLASS(Blueprintable)

class ADVANCEAI_API AGCharacter : public ACharacter, public IGamePlayTagInterFaces, public IGCharacterInterface
{
	GENERATED_BODY()
//variables
public:
	AGCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimInstance* AnimInstance;

	protected:
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	FGameplayTagContainer ActiveGameplayTagContainer;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTagContainer BannedTagContainer;

	//Primary Weapon ref
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	AGBaseGun* Weapon;
	
	//Character AnimInstance of SkeletalMesh

protected:
	
	
	//Used by ABP_CharacterRifle to Set the StandeMode of Character. Crouching or Running Position
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStanceMode StanceMode;

	//Speed for Crouching and Walking Speed. Crouching = Walking
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
	float CrouchAndWalkSpeed;
	
	//Team Number. To Identify An Enemy or Friend
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TeamNumber;

	//Max Running Speed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
	float MaxRunSpeed;

	//Max SprintSpeed
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Speed)
     float MaxSprintSpeed;
	
	//Weapon needs a skeletal mesh bone or socket to be attached to player's  hand, hip or back. Name of the Bone or Socket. Exectly in the Skeletal Mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponSocketBoneName = FName("hand_rSocket");

	//From Run Stance to Walk Stance. Vice verse. Flip flop bool to Changes Stance between Walk mode and run mode
	UPROPERTY()
	bool bWalkingFlipFlop = false;

	//From Run Stance to Crouch Stance. Vice verse. Flip flop bool to Changes Stance between Crouch mode and run mode
	UPROPERTY()
	bool bCrouchingFlipFlop = false;

	//Input the Gun Animation Blueprint which will play after picking up a rifle or gun
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Animation")
	TSubclassOf<UAnimInstance> GunAnimClass;

	//Fill this up with the Enemy of this Character
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* TargetActor;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UGHealthAndDamageSystem* HealthAndDamageSystem;

	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	TMap<FGameplayTag, FMontage> AnimMontageContainers;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Animation Damage reaction Tag")
	FGameplayTag ProjectileReactionTag = FGameplayTag::RequestGameplayTag("Animation.ProjectileDamageReaction");

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Animation Damage reaction Tag")
	FGameplayTag ExplosionReactionTag  = FGameplayTag::RequestGameplayTag("Animation.ExplosionReaction");

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Animation Damage reaction Tag")
	FGameplayTag MeleeReactionTag  = FGameplayTag::RequestGameplayTag("Animation.MeleeDamageReaction");

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category="Animation Damage reaction Tag")
	FGameplayTag DeathTag = FGameplayTag::RequestGameplayTag("Animation.DeathAnim");

   UPROPERTY(EditDefaultsOnly)
	float LifeSpanAfterdeath = 10;
	
	UPROPERTY(EditDefaultsOnly)
	bool IsCharacterAI = false;
	


	


	

//Variables End

	
	
//Functions
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 void OnDamageResponse(EDamageResponse DamageResponse, AActor* DamageCauser, EDamageType DamageType);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	 void OnDeath();
    protected:
	
	void BeginPlay() override;

	//Generic Function from Chaning Stance with a Stance Tag walk, sprint, crouch and run
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnStanceChange(FGameplayTag StanceTag);

	//Call to Toggle sprint mode
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StanceChangeSprint();

	//Call to Toggle  run mode
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StanceChangeRunning();

	//Call to Toggle betwwen Walk and Run. First Toggle Walk mode activates
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StanceChangeWalking();

	//Call to Toggle between Run and Crouch. First Toggle Crouch mode activates
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StanceChangeCrouching();

	//Add Tag from ActiveGameplayTagContainer(Interface function)
	UFUNCTION(BlueprintCallable)
	void AddTag_Implementation(const FGameplayTag AddTag) override;

	//Remove Tag from ActiveGameplayTagContainer(Interface function)
	UFUNCTION(BlueprintCallable)
	void RemoveTag_Implementation(const FGameplayTag RemoveTag) override;

	//Call to Play Animation. Use FMontage to play any anim, section, in any rate(Interface function)
	UFUNCTION(BlueprintCallable)
	void PlayAnimation_Implementation(const FMontage& AnimMontage) override;

	//Call to End the Given Montage.(Interface function)
	UFUNCTION(BlueprintCallable)
	void MontageStop_Implementation(UAnimMontage* AnimMontage) override;

	//Automatically Call when Montage stops normally or Interrupted.(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnMontageEnded(UAnimMontage* Montage, bool Interrupted);
	
	//Audomatically Call when Montage Ends normally without Interruption.(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MontageEnd();

	//Automatically calls when Montage gets interrupted.(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MontageInterrupted();

	//Automatically call when rifle discharges bullet(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnUpdateAmmoEvent(const int32& Ammo);

	//Call to Bind Delegates
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void FunctionBinding();

	//Call to make the weapon Invisible or Visible. First Invisible
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HideWeapoon(bool HidWeapon);

	//Call to Trigger Weapon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GunTrigger();

	//Call to Release the Trigger of Weapon
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GunTriggerEnd();

	//Call to Reload the Weapon manually and Automatically
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Reloading();

	//Switch the guns mode from Auto->Brust->Single. Starts with Single
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SwitchGunMode();

	//Spawns Gun In Hand
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
    void SpawnPrimaryWeapon(TSubclassOf<AGBaseGun> WeaponClass);

	//Creates the Main HUD of the Player
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void CreateMainWidget();

	//Automatically Executes when gun Discharges.(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFireStarted_Event();

	//Automatically Executes when gun Stop Discharging(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnFireEnded_Event();

	//Automatically Executes after finishing the Realod Anim(Delegate)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnReloadFinished_Event(const int32& Ammo, const int32& StorageAmmo);

	//Delegate of the Interaction System. Used to Setup the pop up Widget and Show items name
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void WidgetInteract(AActor* InteractActor);

	//Delegate of the interaction System. After Updating the interaction trace, if no object is found to interact then NoInteraction will Execute
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void NoInteraction();

	//Delegate of Gunsystem. Executes when after collecting Ammunation to fill the storage
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateStorage_Event(int32 CurrentStorage);

	//Delegate of the DamageSystem. Executes when Damage Recieved.
	

	//Call to Bind all the Delegate of GunSystem Component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GunSystemBinding();

	//Call to Bind all the Delegate of InteractionSystem Component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void InteractionSystemBinding();

	// Call to Bind all the Delegate of Anim Instance of the Skeletalmesh Component
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AnimationBinding();

	//For ThirdPerson. Call to Update the CameraBoom Socket Offset from NonAgro position to rifle Handling Position
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateCameraBoomForCrouchFunction();

	//For ThirdPerson. Call to Update the CameraBoom Socket Offset from Run Stance Position to Crouch Position
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void UpdateCameraBoomForCrouchReverseFunction();

	//Character Interface. Sets Characters Speed.
	void SetSpeed_Implementation(float Speed) override;

	//GameplayTag Interfae. Gets this Characters ActiveGameplayTagContainer.
	FGameplayTagContainer GetActiveGamePlayTagContainer_Implementation() override;

	//Executes after Interacting with a Weapon
	void EquipWeapon_Implementation(TSubclassOf<AGBaseGun> WeaponClass) override;

	//Gets this Character.
	AGCharacter* GetCharacter_Implementation() override;

	//
	void StorageInteraction_Implementation(const int32& UpdateAmmo) override;
	
	////Logic Used by to spawn Weapon in hand after interacting with weapon
	void SpawnWeaponLogic(TSubclassOf<AGBaseGun> WeaponClass);

	//Call to Change Stance Tag on GameplayTagContainer and Change the Speed of the Character.
	void SwitchOnStance(const FGameplayTag& SwitchTag, float Speed);

	//Gets this Characters Team Number
	int32 GetTeamNumber_Implementation() override;
	int32 GetTeamNumberWithInterface(AActor* OtherActor);

	//if true, then Actors are hostile. if false then friendly. Call on your self most of the time
	bool CheckifHostile_Implementation(AActor* OtherActor) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	FTransform GetLeftIKTransform();

	void GetWeapon_Implementation(AGBaseGun*& AsWeapon) override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	TArray<FMontage> ChracterMontages;

	void OnConstruction(const FTransform& Transform) override;
	
	void DamageSystemBinding();

	

	bool TakeDamage_Implementation(const FDamageInfo& DamageInfo) override;

	void PlayDamageResponseAnim(EDamageType DamageType);

	

	

	
};


