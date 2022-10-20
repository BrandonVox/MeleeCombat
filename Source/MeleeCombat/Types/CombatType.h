#pragma once

UENUM(BlueprintType)
enum class ECombatType : uint8
{
	ECT_None UMETA(DisplayName = "None"),
	ECT_LightSword UMETA(DisplayName = "Light Sword"),
	ECT_GreatSword UMETA(DisplayName = "Great Sword"),
	ECT_MAX UMETA(DisplayName = "DefaultMAX")
};