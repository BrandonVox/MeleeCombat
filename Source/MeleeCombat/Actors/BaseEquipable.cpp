// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEquipable.h"
#include "MeleeCombat/MeleeCombatCharacter.h"
#include "Components/SkeletalMeshComponent.h"


// Sets default values
ABaseEquipable::ABaseEquipable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	ItemSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ItemSkeletalMesh"));
	ItemStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMesh"));
	ItemSkeletalMesh->SetupAttachment(Root);
	ItemStaticMesh->SetupAttachment(Root);

}

void ABaseEquipable::OnEquipped()
{
	bIsEquipped = true;



}

void ABaseEquipable::OnUnequipped()
{
	if (bIsEquipped)
	{
		bIsEquipped = false;
	}
}

UPrimitiveComponent* ABaseEquipable::GetItemMesh() const
{
	//if (ItemSkeletalMesh->b)
	//{
	//	return ItemSkeletalMesh;
	//}
	return ItemStaticMesh;
}


