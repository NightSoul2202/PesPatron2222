#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include <Interfaces/OnlinePresenceInterface.h>
#include "Interfaces/OnlineLeaderboardInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "SessionManager.h"
#include "MainMenu.h"
#include "PauseMenu.h"
#include "SettingsMenu.h"
#include "LoginMenu.h"
#include "LevelSelectMenu.h"
#include "EOSGameInstance.generated.h"

/**
 *
 */
UCLASS()
class SKGSFEXAMPLE_API UEOSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UEOSGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable)
	void Login();

	UFUNCTION(BlueprintCallable)
	void ReLoginSessionTry();

	void ShowWidget(TSubclassOf<UUserWidget> WidgetClass, UUserWidget*& Menu);

	UFUNCTION(BlueprintCallable)
	void ShowMainWidget();

	UFUNCTION(BlueprintCallable)
	void ShowLoginWidget();

	UFUNCTION(BlueprintCallable)
	void ShowLevelSelectWidget();

	UFUNCTION(BlueprintCallable)
	void ShowPauseWidget();

	UFUNCTION(BlueprintCallable)
	void ShowSettingsWidget();

	void BackToPreviousWidget();

	UFUNCTION(BlueprintCallable)
	void WriteNewRecordToLeaderboard(float Point);

	UFUNCTION(BlueprintCallable)
	void ReadNewRecordToLeaderboard();

	FString GetNickName();

protected:
	virtual void Init() override;
	virtual void Shutdown() override;

	TSubclassOf<class UMainMenu> MainMenuClass;
	TSubclassOf<class ULoginMenu> LoginMenuClass;
	TSubclassOf<class UPauseMenu> PauseMenuClass;
	TSubclassOf<class USettingsMenu> SettingsMenuClass;
	TSubclassOf<class ULevelSelectMenu> LevelSelectMenuClass;

private:
	class IOnlineSubsystem* onlineSubsystem;
	TSharedPtr<IOnlineLeaderboards, ESPMode::ThreadSafe> LeaderboardsInterface;
	TSharedPtr<class IOnlineIdentity, ESPMode::ThreadSafe> identityPtr;
	TSharedPtr<class IOnlineSession, ESPMode::ThreadSafe> Sessions;

	FTimerHandle PingTimerHandle;
	USessionManager* SessionManager;

	UUserWidget* CurrentMenu = nullptr;

	UUserWidget* PreviousMenu = nullptr;

	UMainMenu* MainMenu;
	ULoginMenu* LoginMenu;
	UPauseMenu* PauseMenu;
	USettingsMenu* SettingsMenu;
	ULevelSelectMenu* LevelSelectMenu;

	FString userId = "";

	bool isActiveAccount = false;
	bool isLogged = false;

	UFUNCTION()
	void PingServer();

	void LoginCompleted(int numOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
};
