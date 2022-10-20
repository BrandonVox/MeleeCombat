// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Types/CombatType.h"
#include "CombatAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class MELEECOMBAT_API UCombatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void UpdateCombatType(ECombatType NewCombatType);
private:
	UPROPERTY(BlueprintReadOnly, meta = (AllowPrivateAccess = "true"), Category = Combat)
	ECombatType CombatType;
};
