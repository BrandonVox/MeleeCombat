// Copyright Epic Games, Inc. All Rights Reserved.

#include "MeleeCombatCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "Actors/BaseWeapon.h" // 

#include "Engine/SkeletalMeshSocket.h" // 

#include "Components/CombatComponent.h" // 

#include "CombatAnimInstance.h" // 

#include "Kismet/GameplayStatics.h"

#include "Sound\SoundCue.h"// 
#include "particles/ParticleSystemComponent.h"
#include "GameFramework/PlayerController.h" //

#include "TimerManager.h" // 



AMeleeCombatCharacter::AMeleeCombatCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm
	
	// Actor Components
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

}

void AMeleeCombatCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CombatComponent)
	{
		CombatComponent->SetCombatCharacter(this);
	}

}

void AMeleeCombatCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMeleeCombatCharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("ToggleCombat", IE_Pressed, this, &AMeleeCombatCharacter::ToggleCombatButtonPressed);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AMeleeCombatCharacter::InteractButtonPressed);
	PlayerInputComponent->BindAction("Light Attack", IE_Pressed, this, &AMeleeCombatCharacter::LightAttackButtonPressed);
	PlayerInputComponent->BindAction("Heavy Attack", IE_Pressed, this, &AMeleeCombatCharacter::HeavyAttackButtonPressed);
	PlayerInputComponent->BindAction("Charged Attack", IE_Pressed, this, &AMeleeCombatCharacter::ChargedAttackButtonPressed);
	PlayerInputComponent->BindAction("Charged Attack", IE_Released, this, &AMeleeCombatCharacter::ChargedAttackButtonReleased);
	PlayerInputComponent->BindAction("Dodge", IE_Pressed, this, &AMeleeCombatCharacter::DodgeButtonPressed);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMeleeCombatCharacter::SprintButtonPressed);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMeleeCombatCharacter::SprintButtonReleased);
	PlayerInputComponent->BindAction("Toggle Walk", IE_Pressed, this, &AMeleeCombatCharacter::ToggleWalkButtonPressed);

	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AMeleeCombatCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AMeleeCombatCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AMeleeCombatCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AMeleeCombatCharacter::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AMeleeCombatCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AMeleeCombatCharacter::TouchStopped);
}

void AMeleeCombatCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Remove Dynamic???
	OnTakePointDamage.AddDynamic(this, &AMeleeCombatCharacter::OnReceivedPointDamage);

	Health = MaxHealth;
	// update hud health

	// Default Speed
	CurrentSpeedMode = ESpeedMode::ESM_Jogging;
	GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
}

void AMeleeCombatCharacter::ToggleCombatButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->ToggleCombat();
	}
}

void AMeleeCombatCharacter::PlayAnimationMontage(UAnimMontage* AnimationMontageToPlay)
{
	UCombatAnimInstance* CombatAnimInstance = Cast<UCombatAnimInstance>(GetMesh()->GetAnimInstance());
	if (CombatAnimInstance && AnimationMontageToPlay)
	{
		CombatAnimInstance->Montage_Play(AnimationMontageToPlay);
	}
}

void AMeleeCombatCharacter::InteractButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->Interact();
	}

}

void AMeleeCombatCharacter::LightAttackButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->Attack(false, EAttackType::EAT_Light);
	}
}

void AMeleeCombatCharacter::HeavyAttackButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->Attack(false, EAttackType::EAT_Heavy);
	}
}

void AMeleeCombatCharacter::ChargedAttackButtonPressed()
{
	// Timer v
	GetWorldTimerManager().SetTimer(
		ChargedAttackTimer,
		this,
		&AMeleeCombatCharacter::ChargedAttackTimerFinished,
		ChargeTime
	);
}

void AMeleeCombatCharacter::ChargedAttackTimerFinished()
{
	if (CombatComponent)
	{
		CombatComponent->Attack(false, EAttackType::EAT_Charged);
	}
	// Co can phai clear timer khong?
}

void AMeleeCombatCharacter::ChargedAttackButtonReleased()
{
	// If timer is running -> clear timer
	if( GetWorldTimerManager().IsTimerActive(ChargedAttackTimer) )
	{
		GetWorldTimerManager().ClearTimer(ChargedAttackTimer);
	}
}

void AMeleeCombatCharacter::DodgeButtonPressed()
{
	if (CombatComponent)
	{
		CombatComponent->Dodge(true);
	}
}

void AMeleeCombatCharacter::SprintButtonPressed()
{
	if (CurrentSpeedMode != ESpeedMode::ESM_Sprinting)
	{
		CurrentSpeedMode = ESpeedMode::ESM_Sprinting; 
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	}

}

void AMeleeCombatCharacter::SprintButtonReleased()
{
	if (CurrentSpeedMode == ESpeedMode::ESM_Sprinting)
	{
		CurrentSpeedMode = ESpeedMode::ESM_Jogging;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void AMeleeCombatCharacter::ToggleWalkButtonPressed()
{
	if (CurrentSpeedMode == ESpeedMode::ESM_Sprinting)
	{
		return;
	}

	if (CurrentSpeedMode == ESpeedMode::ESM_Jogging)
	{
		CurrentSpeedMode = ESpeedMode::ESM_Walking;
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
	else // walk change to jog
	{
		CurrentSpeedMode = ESpeedMode::ESM_Jogging;
		GetCharacterMovement()->MaxWalkSpeed = JogSpeed;
	}
}

void AMeleeCombatCharacter::AttachWeaponToSocket(ABaseWeapon* WeaponToAttach, const FName& SocketName)
{
	const USkeletalMeshSocket* WeaponSocket = GetMesh()->GetSocketByName(SocketName);
	if (WeaponSocket && WeaponToAttach)
	{
		WeaponSocket->AttachActor(WeaponToAttach, GetMesh());
	}
}

void AMeleeCombatCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void AMeleeCombatCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void AMeleeCombatCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMeleeCombatCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AMeleeCombatCharacter::MoveForward(float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void AMeleeCombatCharacter::MoveRight(float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}

void AMeleeCombatCharacter::OnReceivedPointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	if (DamagedActor)
	{
		FString ActorName = DamagedActor->GetFName().ToString();
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString("On Take Damage"));
		// GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ActorName);
	}

	UGameplayStatics::PlaySoundAtLocation(this, HitSound, HitLocation);
	UGameplayStatics::SpawnEmitterAtLocation( GetWorld(), ImpactParticle, HitLocation, FRotator());


	// Health
	Health = FMath::Clamp(Health - Damage, 0.f, MaxHealth);

	// Update HUD health here

	if (Health <= 0.f)
	{
		CombatComponent->SetCombatState(ECombatState::ECS_Dead);
	}

	if (CombatComponent->GetCombatState() != ECombatState::ECS_Dead)
	{
		PlayAnimationMontage(HitReactAM);
		CombatComponent->SetCombatState(ECombatState::ECS_Hitting);
	}
	else
	{
		// Play Dead Animations
		EnableRagdoll();
	}


}

void AMeleeCombatCharacter::Jump()
{
	if (CombatComponent->GetCombatState() != ECombatState::ECS_Dodging)
	{
		if (CombatComponent->GetCombatState() == ECombatState::ECS_Attacking)
		{
			// Cancel All Anim montages playing
			UCombatAnimInstance* CombatAnimInstance = Cast<UCombatAnimInstance>(GetMesh()->GetAnimInstance());
			if (CombatAnimInstance && CombatAnimInstance->IsAnyMontagePlaying())
			{
				CombatAnimInstance->StopAllMontages(0.3f);
				CombatComponent->ResetCombat();
			}
		}
		Super::Jump();
	}
}

void AMeleeCombatCharacter::DeadTimerFinished()
{
	// Destroy weapon
	if (CombatComponent)
	{
		if (CombatComponent->GetMainWeapon())
		{
			CombatComponent->GetMainWeapon()->OnUnequipped(); 
			CombatComponent->GetMainWeapon()->Destroy();
		}
	}
	Destroy();
}




void AMeleeCombatCharacter::EnableRagdoll()
{
	PelvisBoneName = FName("pelvis");
	CameraBoom->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepWorldTransform, PelvisBoneName);
	CameraBoom->bDoCollisionTest = false;

	GetCharacterMovement()->DisableMovement();
	GetCharacterMovement()->StopMovementImmediately();
	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		DisableInput(PC);
	}

	GetMesh()->SetCollisionProfileName(FName("ragdoll"), true);
	GetMesh()->SetAllBodiesBelowSimulatePhysics(PelvisBoneName, true, true);
	GetMesh()->SetAllBodiesBelowPhysicsBlendWeight(PelvisBoneName, 1.f, false, true);

	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	GetWorldTimerManager().SetTimer(
		DeadTimer,
		this,
		&AMeleeCombatCharacter::DeadTimerFinished,
		DeadTime
	);

}
