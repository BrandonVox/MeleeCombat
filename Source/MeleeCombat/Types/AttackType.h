#pragma once

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	EAT_None UMETA(DisplayName = "None"),
	EAT_Light UMETA(DisplayName = "Light"),
	EAT_Strong UMETA(DisplayName = "Strong"),
	EAT_Charged UMETA(DisplayName = "Charged"),
	EAT_Falling UMETA(DisplayName = "Falling"),
	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};