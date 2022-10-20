#pragma once

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Free UMETA(DisplayName = "Free"),
	ECS_Attacking UMETA(DisplayName = "Attacking"),
	ECS_Hitting UMETA(DisplayName = "Hitting"),
	ECS_TogglingCombat UMETA(DisplayName = "Toggling Combat"),
	ECS_Dodging UMETA(DisplayName = "Dodging"),
	ECS_Dead UMETA(DisplayName = "Dead"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};