// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseEquipable.generated.h"


class USceneComponent;
class UPrimitiveComponent;
class USkeletalMeshComponent;
class UStaticMeshComponent;
class AMeleeCombatCharacter;

UCLASS()
class MELEECOMBAT_API ABaseEquipable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseEquipable();

protected:
	// Called when the game starts or when spawned

public:	
	// Called every frame
	virtual void OnEquipped();
	virtual void OnUnequipped();
	UFUNCTION(BlueprintCallable)
	virtual UPrimitiveComponent* GetItemMesh() const;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Initialization)
	FName SideSocketName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Initialization)
	FName HandSocketName;

protected: 
	UPROPERTY()
	AMeleeCombatCharacter* CombatCharacter;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root; 
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* ItemSkeletalMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* ItemStaticMesh; 
	bool bIsEquipped = false;

public:
	FORCEINLINE void SetIsEquipped(bool NewValue) { bIsEquipped = NewValue; }
	FORCEINLINE bool IsEquipped() const { return bIsEquipped; }
};
