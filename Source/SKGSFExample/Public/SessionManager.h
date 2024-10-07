#pragma once

#include "CoreMinimal.h"
#include "Http.h"
#include "LeaderboardRecord.h"
#include "SessionManager.generated.h"

UCLASS()
class SKGSFEXAMPLE_API USessionManager : public UObject
{
    GENERATED_BODY()

public:
    void CheckAccountStatus(const FString& AccountId, TFunction<void(bool)> Callback);
    void RegisterNewSession(const FString& AccountId, TFunction<void(bool)> Callback);
    void LogoutSession(const FString& AccountId, TFunction<void(bool)> Callback);
    void AddRecord(const FString& AccountId, const FString& LevelName, float RecordMax, TFunction<void(bool)> Callback);

    void GetLeaderboard(const FString& LevelName, TFunction<void(TArray<FString>, TArray<float>)> Callback);

    void PingServer(TFunction<void(bool)> Callback);

    void SetAccountId(FString AccountId);

    void GetAllLeaderboardRecords(TFunction<void(bool, const TArray<FLeaderboardRecord>&)> Callback);

    void OnGetAllLeaderboardRecordsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(bool, const TArray<FLeaderboardRecord>&)> Callback);

private:
    void OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(bool)> Callback);
    void OnGetLeaderboardResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(TArray<FString>, TArray<float>)> Callback);
    void OnPingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(bool)> Callback);

    FString accountId;
};