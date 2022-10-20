// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MeleeCombat/Interfaces/InteractableInterface.h"
#include "Pickup.generated.h"

class ABaseWeapon;
UCLASS()
class MELEECOMBAT_API APickup : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// Interface
	virtual void Interact_Implementation(class AActor* Caller);

protected:
	// Called when the game starts or when spawned

public:	
	// Called every frame

private:
	UPROPERTY(EditAnywhere)
	class USphereComponent* OverlapSphere;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<ABaseWeapon> WeaponClass;
};
