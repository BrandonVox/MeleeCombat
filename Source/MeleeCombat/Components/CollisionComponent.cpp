// Fill out your copyright notice in the Description page of Project Settings.


#include "CollisionComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MeleeCombat/Actors/BaseWeapon.h"

UCollisionComponent::UCollisionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


// Called every frame
void UCollisionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsEnablingCollision)
	{
		TraceCollision();
	}
}

void UCollisionComponent::TraceCollision()
{
	if (!Weapon)
	{
		return;
	}

	TArray<FHitResult> HitResults; 

	bool bHitSomething = UKismetSystemLibrary::SphereTraceMultiForObjects
	(
		Weapon,
		Weapon->GetTraceStartLocation(),
		Weapon->GetTraceEndLocation(),
		TraceRadius,
		TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	if (bHitSomething)
	{
		for (const FHitResult& HR : HitResults)
		{
			if (HR.bBlockingHit)
			{
				LastHitResult = HR;
				if (LastHitResult.GetActor())
				{
					if (!AlreadyHitActors.Contains(LastHitResult.GetActor()))
					{
						HitActorDelegate.Broadcast(LastHitResult);
						AlreadyHitActors.Emplace(LastHitResult.GetActor());
					}
				}
			}
		}
	}


	
}

void UCollisionComponent::AddToActorsToIgnore(AActor* Value)
{
	if (Value)
	{
		ActorsToIgnore.AddUnique(Value);
	}
}

void UCollisionComponent::RemoveActorsToIgnore(AActor* Value)
{
	if (Value)
	{
		ActorsToIgnore.Remove(Value);
	}
}

void UCollisionComponent::EnableCollision()
{
	ClearHitActors();
	bIsEnablingCollision = true;
}

void UCollisionComponent::DisableCollision()
{
	bIsEnablingCollision = false;
}

void UCollisionComponent::ClearHitActors()
{
	AlreadyHitActors.Empty();
}

