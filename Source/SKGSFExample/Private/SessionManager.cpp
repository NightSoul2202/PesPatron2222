#include "SessionManager.h"
#include "Json.h"
#include "JsonUtilities.h"
#include "LeaderboardRecord.h"
#include <Kismet/GameplayStatics.h>

void USessionManager::CheckAccountStatus(const FString& AccountId, TFunction<void(bool)> Callback)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("account_id"), AccountId);

    FString ContentString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnResponseReceived, Callback);
    Request->SetURL(TEXT("http://127.0.0.1:5000/check_account"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(ContentString);
    Request->ProcessRequest();
}

void USessionManager::RegisterNewSession(const FString& AccountId, TFunction<void(bool)> Callback)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("account_id"), AccountId);

    FString ContentString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnResponseReceived, Callback);
    Request->SetURL(TEXT("http://127.0.0.1:5000/register_session"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(ContentString);
    Request->ProcessRequest();
}

void USessionManager::LogoutSession(const FString& AccountId, TFunction<void(bool)> Callback)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("account_id"), AccountId);

    FString ContentString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnResponseReceived, Callback);
    Request->SetURL(TEXT("http://127.0.0.1:5000/logout_session"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(ContentString);
    Request->ProcessRequest();
}

void USessionManager::AddRecord(const FString& AccountId, const FString& LevelName, float RecordMax, TFunction<void(bool)> Callback)
{
    TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
    JsonObject->SetStringField(TEXT("account_id"), AccountId);
    JsonObject->SetStringField(TEXT("level_name"), LevelName);
    JsonObject->SetNumberField(TEXT("recordMax"), RecordMax);

    FString ContentString;
    TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&ContentString);
    FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnResponseReceived, Callback);
    Request->SetURL(TEXT("http://127.0.0.1:5000/add_record"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(ContentString);
    Request->ProcessRequest();
}

void USessionManager::GetLeaderboard(const FString& LevelName, TFunction<void(TArray<FString>, TArray<float>)> Callback)
{
    FString Url = FString::Printf(TEXT("http://127.0.0.1:5000/get_records?level_name=%s"), *LevelName);

    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnGetLeaderboardResponseReceived, Callback);
    Request->SetURL(Url);
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->ProcessRequest();
}

void USessionManager::PingServer(TFunction<void(bool)> Callback)
{
    FHttpModule* Http = &FHttpModule::Get();
    TSharedRef<IHttpRequest, ESPMode::ThreadSafe> Request = Http->CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnPingResponseReceived, Callback);
    Request->SetURL(TEXT("http://127.0.0.1:5000/ping"));
    Request->SetVerb(TEXT("POST"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->SetContentAsString(FString::Printf(TEXT("{\"account_id\": \"%s\"}"), *accountId));
    Request->ProcessRequest();
}

void USessionManager::SetAccountId(FString AccountId)
{
    this->accountId = AccountId;
}

void USessionManager::GetAllLeaderboardRecords(TFunction<void(bool, const TArray<FLeaderboardRecord>&)> Callback)
{
    TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
    Request->OnProcessRequestComplete().BindUObject(this, &USessionManager::OnGetAllLeaderboardRecordsResponseReceived, Callback);
    Request->SetURL(TEXT("http://127.0.0.1:5000/get_all_leaderboard_records"));
    Request->SetVerb(TEXT("GET"));
    Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));
    Request->ProcessRequest();
}

void USessionManager::OnGetAllLeaderboardRecordsResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(bool, const TArray<FLeaderboardRecord>&)> Callback)
{
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            const TArray<TSharedPtr<FJsonValue>>* RecordsArray;
            if (JsonObject->TryGetArrayField(TEXT("leaderboard"), RecordsArray))
            {
                TArray<FLeaderboardRecord> LeaderboardRecords;

                for (const TSharedPtr<FJsonValue>& RecordValue : *RecordsArray)
                {
                    TSharedPtr<FJsonObject> RecordObject = RecordValue->AsObject();

                    FLeaderboardRecord Record;
                    Record.AccountId = RecordObject->GetStringField(TEXT("account_id"));
                    Record.LevelName = RecordObject->GetStringField(TEXT("level_name"));
                    Record.RecordMax = RecordObject->GetNumberField(TEXT("recordMax"));

                    LeaderboardRecords.Add(Record);
                }

                Callback(true, LeaderboardRecords);
                return;
            }
        }
    }
    Callback(false, TArray<FLeaderboardRecord>());
}

void USessionManager::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(bool)> Callback)
{
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            bool bSuccess = JsonObject->GetStringField(TEXT("status")) == TEXT("success");
            Callback(bSuccess);
            return;
        }
    }
    Callback(false);
}

void USessionManager::OnGetLeaderboardResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(TArray<FString>, TArray<float>)> Callback)
{
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        TSharedPtr<FJsonObject> JsonObject;
        TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
        if (FJsonSerializer::Deserialize(Reader, JsonObject))
        {
            const TArray<TSharedPtr<FJsonValue>>* LeaderboardArray;
            if (JsonObject->TryGetArrayField(TEXT("leaderboard"), LeaderboardArray))
            {
                TArray<FString> AccountIds;
                TArray<float> Records;

                for (const TSharedPtr<FJsonValue>& Value : *LeaderboardArray)
                {
                    TSharedPtr<FJsonObject> EntryObject = Value->AsObject();
                    FString AccountId = EntryObject->GetStringField(TEXT("account_id"));
                    float RecordMax = EntryObject->GetNumberField(TEXT("recordMax"));
                    AccountIds.Add(AccountId);
                    Records.Add(RecordMax);
                }

                Callback(AccountIds, Records);
                return;
            }
        }
    }
    Callback(TArray<FString>(), TArray<float>());
}

void USessionManager::OnPingResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful, TFunction<void(bool)> Callback)
{
    if (bWasSuccessful && Response->GetResponseCode() == 200)
    {
        UE_LOG(LogTemp, Log, TEXT("Ping successful"));
        Callback(true);
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Ping failed"));
        Callback(false);
    }
}
