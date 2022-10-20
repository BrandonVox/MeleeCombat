// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CollisionComponent.generated.h"



DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitActorDelegate, const FHitResult&, LastHitResult);

class ABaseWeapon;
class UPrimitiveComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MELEECOMBAT_API UCollisionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCollisionComponent();

protected:
	// Called when the game starts
	// virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	FHitActorDelegate HitActorDelegate;

	void AddToActorsToIgnore(AActor* Value);
	void RemoveActorsToIgnore(AActor* Value);

	UFUNCTION(BlueprintCallable)
	void EnableCollision();
	UFUNCTION(BlueprintCallable)
	void DisableCollision();
	UFUNCTION(BlueprintCallable)
	void TraceCollision();
	UFUNCTION(BlueprintCallable)
	void ClearHitActors();
private:
	UPROPERTY()
	ABaseWeapon* Weapon;

	// Trace Hit
	UPROPERTY()
	FVector TraceStartLocation;
	UPROPERTY()
	FVector TraceEndLocation;


	UPROPERTY(EditDefaultsOnly, Category = TraceHit)
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = TraceHit)
	TArray<AActor*> ActorsToIgnore;

	UPROPERTY(EditAnywhere, Category = TraceHit)
	float TraceRadius = 20.f;


	// Trace Hit results
	bool bIsEnablingCollision = false;

	UPROPERTY()
	TArray<AActor*> AlreadyHitActors;

	UPROPERTY()
	FHitResult LastHitResult;


public:
	FORCEINLINE void SetWeapon(ABaseWeapon* NewValue) { Weapon = NewValue; }
	FORCEINLINE void SetTraceStartLocation(const FVector& Value) { TraceStartLocation = Value;}
	FORCEINLINE void SetTraceEndLocation(const FVector& Value) { TraceEndLocation = Value; }

	FORCEINLINE ABaseWeapon* GetWeapon() const { return Weapon; }
};
