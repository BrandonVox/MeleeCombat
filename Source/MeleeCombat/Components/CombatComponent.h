// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeleeCombat/Types/CombatState.h"
#include "MeleeCombat/Types/AttackType.h"
#include "CombatComponent.generated.h"

class AMeleeCombatCharacter;
class ABaseWeapon;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBAT_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

public:	
	
	// Called every frame
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void AttackButtonPressed(const bool& bRandomAttack, const EAttackType& TypeOfAttack);
	bool CanAttack(const EAttackType& TypeOfAttack);
	void ToggleCombat();
	void Interact();
	void Dodge(bool bRandomDodge);
	bool CanDodge();

	void SpawnWeapon(TSubclassOf<ABaseWeapon> WeaponClass);

	UFUNCTION(BlueprintCallable)
	void PerformNextAttackPoint(const EAttackType& TypeOfAttack);

	UFUNCTION(BlueprintCallable)
	void OnResetAttack();

	UFUNCTION(BlueprintCallable)
	void OnFinishToggleCombat();


	UFUNCTION(BlueprintCallable)
	void ResetCombat();

	UFUNCTION(BlueprintCallable)
	void RotateCharacterWhenDodging(const float& DeltaTime, const float& InterpSpeed);
private:
	UPROPERTY()
	AMeleeCombatCharacter* CombatCharacter;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ABaseWeapon* MainWeapon;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState = ECombatState::ECS_Free;

	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EAttackType CurrentAttackType;
	

	bool bIsHoldingWeapon = false;
	bool bIsSavingAttack = false;

	// Chi true lan dau neu hold weapon
	bool bCanPerformNextAttack = false;


	int32 AttackIndex = 0;
	int32 DodgeIndex = 0;


public:
	FORCEINLINE ABaseWeapon* GetMainWeapon() const { return MainWeapon; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
	FORCEINLINE EAttackType GetCurrentAttackType() const { return CurrentAttackType; }
	FORCEINLINE bool IsHoldingWeapon() const { return bIsHoldingWeapon; }

	FORCEINLINE void SetCombatCharacter(AMeleeCombatCharacter* NewValue) { CombatCharacter = NewValue; }
	FORCEINLINE void SetMainWeapon(ABaseWeapon* NewValue) { MainWeapon = NewValue; }
	FORCEINLINE void SetIsHoldingWeapon(bool bValue) { bIsHoldingWeapon = bValue; }
	FORCEINLINE void SetCombatState(const ECombatState& NewValue) { CombatState = NewValue; }
	FORCEINLINE void SetCurrentAttackType(const EAttackType& NewValue) { CurrentAttackType = NewValue; }
};
