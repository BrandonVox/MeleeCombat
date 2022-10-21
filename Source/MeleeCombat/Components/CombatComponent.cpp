// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatComponent.h"
#include "MeleeCombat/MeleeCombat.h"
#include "MeleeCombat/MeleeCombatCharacter.h"
#include "MeleeCombat/Actors/BaseWeapon.h"
#include "MeleeCombat/Interfaces/InteractableInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

#include "MeleeCombat/CombatAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"



UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UCombatComponent::SpawnWeapon(TSubclassOf<ABaseWeapon> WeaponClass)
{
	if (!CombatCharacter)
	{
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = CombatCharacter;
	SpawnParams.Instigator = CombatCharacter;
	ABaseWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(WeaponClass, SpawnParams);
	
	if (SpawnedWeapon)
	{
		MainWeapon = SpawnedWeapon;
		MainWeapon->OnEquipped();

		const FName& AttachSocketName = bIsHoldingWeapon? SpawnedWeapon->HandSocketName : SpawnedWeapon->SideSocketName;
		CombatCharacter->AttachWeaponToSocket(SpawnedWeapon, AttachSocketName);
			

		if (CombatCharacter->GetMesh())
		{
			UCombatAnimInstance* CombatAnimInstance =
				Cast<UCombatAnimInstance>(CombatCharacter->GetMesh()->GetAnimInstance());
			if (CombatAnimInstance)
			{
				CombatAnimInstance
					->UpdateCombatType(bIsHoldingWeapon ? SpawnedWeapon->GetCombatType() : ECombatType::ECT_None);
			}
		}
	}
}

void UCombatComponent::Attack(const bool& bRandomAttack, const EAttackType& TypeOfAttack)
{
	if (bCanPerformNextAttack == false)
	{
		bIsSavingAttack = true;
		// 
		if (CurrentAttackType != TypeOfAttack)
		{
			CurrentAttackType = TypeOfAttack;
			AttackIndex = 0;
		}
		return;
	}

	if (CanAttack(TypeOfAttack))
	{
		// Get Correct Attack Anim Montage
		
		const TArray<UAnimMontage*>& AttackArray = MainWeapon->GetAttackAnimMontages(TypeOfAttack);
		UAnimMontage* AttackAM;
		if (bRandomAttack)
		{
			int32 RandomIndex = FMath::RandRange(0, AttackArray.Num() - 1);
			AttackAM = AttackArray[RandomIndex];
		}
		else
		{
			AttackAM = AttackArray[AttackIndex];
			++AttackIndex;
			if (AttackIndex > AttackArray.Num() - 1)
			{
				AttackIndex = 0;
			}
		}

		// Actual Attack
		if (AttackAM)
		{
			CurrentAttackType = TypeOfAttack;
			CombatCharacter->PlayAnimationMontage(AttackAM);
			CombatState = ECombatState::ECS_Attacking;
			bCanPerformNextAttack = false;
		}
	}
}

bool UCombatComponent::CanAttack(const EAttackType& TypeOfAttack)
{
	// neu dang nhay thi return ve false luon, khong mau me
	if (CombatCharacter && CombatCharacter->GetCharacterMovement()->IsFalling())
	{
		return false;
	}
	
	

	return CombatCharacter 
		&& bIsHoldingWeapon
		&& MainWeapon 
		&& !MainWeapon->GetAttackAnimMontages(TypeOfAttack).IsEmpty() 
		&& (bCanPerformNextAttack || CombatState == ECombatState::ECS_Free);
}


// Khi dang attack nhung o 1 diem nao do co the thuc hien attack tiep theo ma khong can
// phai doi cho attack nay ket thuc
// co nghia la van dang attack
// khong attack chi dien ra khi reset combat (doi 1 diem khi animation gan ket thuc)
void UCombatComponent::PerformNextAttackPoint(const EAttackType& TypeOfAttack)
{
	if (bIsSavingAttack)
	{
		Attack(false, TypeOfAttack);
		bIsSavingAttack = false;
		bCanPerformNextAttack = true;
	}
}

void UCombatComponent::OnResetAttack()
{
	CombatState = ECombatState::ECS_Free;
	bIsSavingAttack = false;
	AttackIndex = 0;
}

void UCombatComponent::ToggleCombat()
{	
	if (!MainWeapon|| !CombatCharacter || !CombatCharacter->GetMesh())
	{
		return;
	}

	// Unsafe -> Fix Check if Combat state free
	UCombatAnimInstance* CombatAnimInstance = Cast<UCombatAnimInstance>(CombatCharacter->GetMesh()->GetAnimInstance()); 
	if (CombatAnimInstance->IsAnyMontagePlaying())
	{
		return;
	}

	if (!bIsHoldingWeapon)
	{
		if (MainWeapon->GetDrawAM())
		{
			CombatCharacter->PlayAnimationMontage(MainWeapon->GetDrawAM());
			bIsHoldingWeapon = true;
			bCanPerformNextAttack = true;
			CombatState = ECombatState::ECS_TogglingCombat;
		}
	}
	else
	{
		if (MainWeapon->GetDisarmAM())
		{
			CombatCharacter->PlayAnimationMontage(MainWeapon->GetDisarmAM());
			bIsHoldingWeapon = false;
			bCanPerformNextAttack = false; // boi vi minh da bo weapon!!!
			CombatState = ECombatState::ECS_TogglingCombat;
		}
	}
}

void UCombatComponent::OnFinishToggleCombat()
{
	CombatState = ECombatState::ECS_Free;
}

void UCombatComponent::Interact()
{
	if (!CombatCharacter)
	{
		return;
	}

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Emplace(ECC_Interactable);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Emplace(CombatCharacter);

	FHitResult HitResult;

	bool bHitSomething = UKismetSystemLibrary::SphereTraceSingleForObjects
	(
		CombatCharacter,
		CombatCharacter->GetActorLocation(),
		CombatCharacter->GetActorLocation(),
		100.f,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (HitResult.bBlockingHit && HitResult.GetActor())
	{
		TScriptInterface<IInteractableInterface> Interactable = HitResult.GetActor();
		if (Interactable != nullptr)
		{
			Interactable->Execute_Interact(Interactable.GetObject(), CombatCharacter);
		}
	}
}

void UCombatComponent::Dodge(bool bRandomDodge)
{
	// Consider delete isholdingweapon
	if (CanDodge())
	{
		UAnimMontage* DodgeAM;
		if (bRandomDodge)
		{
			int32 RandomIndex = FMath::RandRange(0, MainWeapon->GetDodgeAMs().Num() - 1);
			DodgeAM = MainWeapon->GetDodgeAMs()[RandomIndex];
		}
		else
		{
			DodgeAM = MainWeapon->GetDodgeAMs()[DodgeIndex];
			++DodgeIndex;
			if (DodgeIndex > MainWeapon->GetDodgeAMs().Num() - 1)
			{
				DodgeIndex = 0;
			}
		}

		if (DodgeAM)
		{
			CombatCharacter->PlayAnimationMontage(DodgeAM);
			CombatState = ECombatState::ECS_Dodging;
		}
	}

}

bool UCombatComponent::CanDodge()
{
	bool bFittedCombatState = CombatState == ECombatState::ECS_Free || CombatState == ECombatState::ECS_Attacking;
	return CombatCharacter 
		&& MainWeapon 
		&& !MainWeapon->GetDodgeAMs().IsEmpty() 
		&& bFittedCombatState;
}


void UCombatComponent::ResetCombat()
{
	DodgeIndex = 0;
	AttackIndex = 0;
	bIsSavingAttack = false;
	CombatState = ECombatState::ECS_Free;
	bCanPerformNextAttack = true;
	CurrentAttackType = EAttackType::EAT_None; 
}

void UCombatComponent::RotateCharacterWhenDodging(const float& DeltaTime, const float& InterpSpeed)
{
	if (!CombatCharacter || !CombatCharacter->GetCharacterMovement())
	{
		return;
	}

	// calculate rotation
	FRotator DodgeRotation;
	if (CombatCharacter->GetCharacterMovement()->GetLastInputVector().Size() > 0.0001)
	{
		DodgeRotation = UKismetMathLibrary::MakeRotFromX(CombatCharacter->GetLastMovementInputVector());
	}
	else
	{
		DodgeRotation = CombatCharacter->GetActorRotation();
	}

	DodgeRotation = FMath::RInterpTo(CombatCharacter->GetActorRotation(), DodgeRotation, DeltaTime, InterpSpeed);

	CombatCharacter->SetActorRotation(DodgeRotation);
}