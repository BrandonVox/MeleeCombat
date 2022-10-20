// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseEquipable.h"
#include "MeleeCombat/Types/CombatType.h"
#include "BaseWeapon.generated.h"

/**
 * 
 */

class UAnimMontage;
class UCollisionComponent;

UCLASS()
class MELEECOMBAT_API ABaseWeapon : public ABaseEquipable
{
	GENERATED_BODY()

public:
	ABaseWeapon();
	virtual void PostInitializeComponents() override;
	virtual void OnEquipped() override;
	virtual void OnUnequipped() override;


	FVector GetTraceStartLocation() const;
	FVector GetTraceEndLocation() const;

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void OnHitActor(const FHitResult& LastHitResult);

private:
	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Initialization)
	ECombatType CombatType;

	UPROPERTY(BlueprintReadOnly, EditAnywhere, meta = (AllowPrivateAccess = "true"), Category = Initialization)
	float Damage = 20.f;

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* DrawAM;

	UPROPERTY(EditAnywhere, Category = Animations)
	UAnimMontage* DisarmAM;


	UPROPERTY(EditAnywhere, Category = Animations)
	TArray<UAnimMontage*> AttackAMs;

	UPROPERTY(EditAnywhere, Category = Animations)
	TArray<UAnimMontage*> DodgeAMs;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCollisionComponent* CollisionComponent;

	// Trace Hit
	UPROPERTY(EditAnywhere, Category = TraceHit)
	FName StartSocketName;

	UPROPERTY(EditAnywhere, Category = TraceHit)
	FName EndSocketName;

public:
	FORCEINLINE UAnimMontage* GetDrawAM() const { return DrawAM;}
	FORCEINLINE UAnimMontage* GetDisarmAM() const { return DisarmAM; }
	FORCEINLINE ECombatType GetCombatType() const { return CombatType; }

	FORCEINLINE TArray<UAnimMontage*> GetAttackAMs() const { return AttackAMs; }
	FORCEINLINE TArray<UAnimMontage*> GetDodgeAMs() const { return DodgeAMs; }

	FORCEINLINE UCollisionComponent* GetCollisionComponent() const { return CollisionComponent; }
};
