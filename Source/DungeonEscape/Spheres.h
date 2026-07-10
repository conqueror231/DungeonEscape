#pragma once

#include "CoreMinimal.h"
#include "Spheres.generated.h"

UENUM(BlueprintType)
enum class ESphereType : uint8
{
    Quas    UMETA(DisplayName = "Quas"),
    Wex     UMETA(DisplayName = "Wex"),
    Exort   UMETA(DisplayName = "Exort")
};