#pragma once
UENUM(BlueprintType)
enum class ESpeedMode : uint8
{
	ESM_Jogging UMETA(DisplayName = "Jogging"),
	ESM_Walking UMETA(DisplayName = "Walking"),
	ESM_Sprinting UMETA(DisplayName = "Sprinting"),
	ESM_MAX UMETA(DisplayName = "DefaultMAX")
};