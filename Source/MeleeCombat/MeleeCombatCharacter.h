// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Types/SpeedMode.h"
#include "MeleeCombatCharacter.generated.h"

class ABaseWeapon;
class UAnimMontage;
class UCombatComponent;
class USoundCue;

UCLASS(config=Game)
class AMeleeCombatCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
public:
	AMeleeCombatCharacter();
	virtual void PostInitializeComponents() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;

	UFUNCTION(BlueprintCallable)
	void AttachWeaponToSocket(ABaseWeapon* WeaponToAttach, const FName& SocketName);

	void PlayAnimationMontage(UAnimMontage* AnimationMontageToPlay);

	UFUNCTION(BlueprintCallable)
	void EnableRagdoll();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	void ToggleCombatButtonPressed();
	void InteractButtonPressed();
	void LightAttackButtonPressed();
	void DodgeButtonPressed();

	void SprintButtonPressed();
	void SprintButtonReleased();
	void ToggleWalkButtonPressed();


	UFUNCTION()
	void OnReceivedPointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
		FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName,
		FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	virtual void Jump() override;

private:
	void DeadTimerFinished();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	// Not using
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABaseWeapon> DefaultWeaponClass;


	UPROPERTY(EditAnywhere, Category = Combat)
	USoundCue* HitSound;

	UPROPERTY(EditAnywhere, Category = Combat)
	UParticleSystem* ImpactParticle;

	UPROPERTY(EditAnywhere, Category = Combat)
	UAnimMontage* HitReactAM;

	UPROPERTY(EditAnywhere, Category = Stats)
	float JogSpeed = 500.f;
	UPROPERTY(EditAnywhere, Category = Stats)
	float WalkSpeed = 350.f;
	UPROPERTY(EditAnywhere, Category = Stats)
	float SprintSpeed = 700.f;

	UPROPERTY(EditAnywhere, Category = Stats)
	float MaxHealth = 100.f;

	float Health = 100.f;


	UPROPERTY(EditAnywhere)
	FName PelvisBoneName;

	UPROPERTY()
	FTimerHandle DeadTimer;

	UPROPERTY(EditAnywhere, Category = Combat)
	float DeadTime = 3.f;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ESpeedMode CurrentSpeedMode = ESpeedMode::ESM_Jogging;

public:
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComponent; }
};

