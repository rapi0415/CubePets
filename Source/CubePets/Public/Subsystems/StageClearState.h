#pragma once

#include "CoreMinimal.h"
#include "StageClearState.generated.h"

UENUM(BlueprintType)
enum class EStageClearState : uint8
{
    NotCleared,
    Cleared,
    FullyCleared
};

