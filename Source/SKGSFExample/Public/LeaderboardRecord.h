#pragma once

#include "CoreMinimal.h"
#include "LeaderboardRecord.generated.h"

USTRUCT(BlueprintType)
struct FLeaderboardRecord
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category = "Leaderboard")
    FString AccountId;

    UPROPERTY(BlueprintReadWrite, Category = "Leaderboard")
    FString LevelName;

    UPROPERTY(BlueprintReadWrite, Category = "Leaderboard")
    float RecordMax;
};
