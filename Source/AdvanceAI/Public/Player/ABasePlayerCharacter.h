// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "AdvanceAI/Public/Player/GCharacter.h"
#include "ABasePlayerCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class UGInteractionWidget;
class UGMainWidget;
static TAutoConsoleVariable<bool> CVarGameplayTag(TEXT("su.ShowGamePlayTag"), false, TEXT("if true, every tick ,all the tags of container will be printedOnScree"), ECVF_Cheat);
DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ABasePlayerCharacter : public AGCharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UGInteractionWidget* InteractionWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget")
	UGMainWidget* PlayerHud;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Player")
	FVector NormalThirdPersonCamera;
	
	void OnDamageResponse_Implementation(EDamageResponse DamageResponse, AActor* DamageCauser, EDamageType DamageType) override;

	void OnDeath_Implementation() override;

	
   



	
public:
	ABasePlayerCharacter();
	
	

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RunCameraOffset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector CrouchCameraZoomLocation;
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	UFUNCTION(BlueprintNativeEvent)
	void PlayMovementAnimation(bool StartMontage, int32 ForwardDirection, int32 RightDirection);

	UFUNCTION()
	void MoveEnd(const FInputActionValue& Value);

	UFUNCTION(BlueprintCallable)
	void DebugFunction();

	void BeginPlay()override;

	UFUNCTION(BlueprintCallable)
	void SecondDebugFunction();

	bool AnimPlaying = false;

	int32 NCounter = 0;

	

	

protected:

	virtual void NotifyControllerChanged() override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	void Jump() override;
protected:
    void Tick(float DeltaTime) override;

	void GunTrigger_Implementation() override;

	void GunTriggerEnd_Implementation() override;
	
	void Reloading_Implementation() override;

	void SwitchGunMode_Implementation() override;
private:
	UFUNCTION(Exec)
	void SetSpeed(float Speed = 10000);
};

