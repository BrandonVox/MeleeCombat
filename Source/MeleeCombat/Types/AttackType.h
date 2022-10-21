#pragma once

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_Light UMETA(DisplayName = "Light"),
	EAT_Heavy UMETA(DisplayName = "Heavy"),
	EAT_Charged UMETA(DisplayName = "Charged"),
	EAT_Falling UMETA(DisplayName = "Falling"),
	EAT_Sprinting UMETA(DisplayName = "Sprinting"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};