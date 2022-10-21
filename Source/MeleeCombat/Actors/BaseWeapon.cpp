// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseWeapon.h"
#include "MeleeCombat/CombatAnimInstance.h"
#include "MeleeCombat/MeleeCombatCharacter.h"

#include "MeleeCombat/Components/CollisionComponent.h" // 
#include "Kismet/GameplayStatics.h"


ABaseWeapon::ABaseWeapon()
{
	CollisionComponent = CreateDefaultSubobject<UCollisionComponent>(TEXT("CollisionComponent"));
}

void ABaseWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (CollisionComponent)
	{
		CollisionComponent->SetWeapon(this);
	}

}

void ABaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseWeapon::OnHitActor(const FHitResult& LastHitResult)
{
	// Do something when hit something

	AActor* HitActor = LastHitResult.GetActor();
	if (HitActor)
	{
		FString ActorName = HitActor->GetFName().ToString();
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, ActorName);
		}

		if (GetOwner())
		{
			UGameplayStatics::ApplyPointDamage(
				HitActor,
				Damage,
				GetOwner()->GetActorForwardVector(),
				LastHitResult,
				GetInstigatorController(),
				this,
				UDamageType::StaticClass()
				); 
		}		
	}

	
}

void ABaseWeapon::OnEquipped()
{
	Super::OnEquipped();
	// For Weapon Trace
	if (CollisionComponent)
	{
		CombatCharacter = CombatCharacter ? CombatCharacter : Cast<AMeleeCombatCharacter> (GetOwner());
		CollisionComponent->AddToActorsToIgnore(CombatCharacter);

		CollisionComponent->HitActorDelegate.AddDynamic(this, &ABaseWeapon::OnHitActor);
	}
}

void ABaseWeapon::OnUnequipped()
{
	Super::OnUnequipped();
	if (CollisionComponent)
	{		
		CollisionComponent->HitActorDelegate.RemoveDynamic(this, &ABaseWeapon::OnHitActor);
	}
}

FVector ABaseWeapon::GetTraceStartLocation() const
{

	return GetItemMesh()->GetSocketLocation(StartSocketName);
	
}

FVector ABaseWeapon::GetTraceEndLocation() const
{
	return GetItemMesh()->GetSocketLocation(EndSocketName);
}

TArray<UAnimMontage*> ABaseWeapon::GetAttackAnimMontages(const EAttackType& AttackType) const
{
	switch (AttackType)
	{
	case EAttackType::EAT_Light:
		return LightAttackAnimMontages;
		break;
	case EAttackType::EAT_Heavy:
		return HeavyAttackAnimMontages;
		break;
	case EAttackType::EAT_Charged:
		return ChargedAttackAnimMontages;
		break;
	case EAttackType::EAT_Falling:
		return FallingAttackAnimMontages;
		break;
	case EAttackType::EAT_Sprinting:
		return SprintingAttackAnimMontages;
		break;
	}

	return TArray<UAnimMontage*>();
}




