// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MeleeCombat/MeleeCombatCharacter.h"
#include "Components/SphereComponent.h"
#include "MeleeCombat/MeleeCombat.h"
#include "MeleeCombat/Components/CombatComponent.h"
#include "MeleeCombat/Actors/BaseWeapon.h"


// Sets default values
APickup::APickup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	OverlapSphere = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapSphere"));
	OverlapSphere->SetupAttachment(RootComponent);
	OverlapSphere->SetSphereRadius(100.f);

	OverlapSphere->SetCollisionObjectType(ECC_Interactable);
	OverlapSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapSphere->SetCollisionResponseToChannel(ECC_Interactable, ECollisionResponse::ECR_Overlap);

	// OverlapSphere->AddLocalOffset(FVector(0.f, 0.f, 85.f));


	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->SetupAttachment(OverlapSphere);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PickupMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block); 
	// PickupMesh->SetRelativeScale3D(FVector(5.f, 5.f, 5.f));
}

void APickup::Interact_Implementation(AActor* Caller)
{
	UKismetSystemLibrary::PrintString(this, Caller->GetFName().ToString(), true, true, FLinearColor::Blue);


	// Pick up weapon
	AMeleeCombatCharacter* CombatCharacter = Cast<AMeleeCombatCharacter>(Caller);	
	if (!CombatCharacter)
	{
		return;
	}
	UCombatComponent* CombatComponent = CombatCharacter->GetCombatComponent();

	if (CombatComponent)
	{
		if (CombatComponent->GetMainWeapon())
		{
			CombatComponent->GetMainWeapon()->OnUnequipped();
			CombatComponent->GetMainWeapon()->Destroy();
			CombatComponent->SpawnWeapon(WeaponClass);
		}
		else
		{
			CombatComponent->SpawnWeapon(WeaponClass);
		}
	}
}