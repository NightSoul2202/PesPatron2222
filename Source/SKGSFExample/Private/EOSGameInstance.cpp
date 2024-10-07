#include "EOSGameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "List"
#include "HttpModule.h"
#include "Http.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include <Kismet/GameplayStatics.h>
#include "LeaderboardRecord.h"
#include "Components/TextRenderComponent.h"
#include <RecordBoard.h>


UEOSGameInstance::UEOSGameInstance(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UMainMenu> MainMenuBPClass(TEXT("/Game/Blueprint/WBP/WBP_MainMenu"));
	static ConstructorHelpers::FClassFinder<ULoginMenu> LoginMenuBPClass(TEXT("/Game/Blueprint/WBP/WBP_LoginMenu"));
	static ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenuBPClass(TEXT("/Game/Blueprint/WBP/WBP_PauseMenu"));
	static ConstructorHelpers::FClassFinder<USettingsMenu> SettingsMenuBPClass(TEXT("/Game/Blueprint/WBP/WBP_SettingsMenu"));
	static ConstructorHelpers::FClassFinder<ULevelSelectMenu> LevelSelectBPClass(TEXT("/Game/Blueprint/WBP/WBP_LevelSelectMenu"));

	if (!ensure(MainMenuBPClass.Class != nullptr && LoginMenuBPClass.Class != nullptr && PauseMenuBPClass.Class != nullptr && SettingsMenuBPClass.Class != nullptr && LevelSelectBPClass.Class != nullptr)) return;

	MainMenuClass = MainMenuBPClass.Class;
	LoginMenuClass = LoginMenuBPClass.Class;
	PauseMenuClass = PauseMenuBPClass.Class;
	SettingsMenuClass = SettingsMenuBPClass.Class;
	LevelSelectMenuClass = LevelSelectBPClass.Class;
}

void UEOSGameInstance::Login()
{
	if (identityPtr)
	{
		FOnlineAccountCredentials onlineAccountCredentials;
		onlineAccountCredentials.Type = "accountportal";
		onlineAccountCredentials.Id = "";
		onlineAccountCredentials.Token = "";
		identityPtr->Login(0, onlineAccountCredentials);
	}
}

void UEOSGameInstance::ReLoginSessionTry()
{
	if (isLogged)
	{

		UE_LOG(LogTemp, Warning, TEXT("Checking account status for user: %s"), *userId);

		SessionManager = NewObject<USessionManager>();
		SessionManager->SetAccountId(this->userId);

		SessionManager->CheckAccountStatus(userId, [this](bool bIsActive)
			{
				if (bIsActive)
				{
					UE_LOG(LogTemp, Warning, TEXT("Account is already active for user: %s"), *userId);

					isActiveAccount = true;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Registering new session for user: %s"), *userId);

					SessionManager->RegisterNewSession(userId, [this](bool bSuccess)
						{
							if (bSuccess)
							{
								isActiveAccount = false;

								GetWorld()->GetTimerManager().SetTimer(PingTimerHandle, this, &UEOSGameInstance::PingServer, 10.0f, true);

								UE_LOG(LogTemp, Warning, TEXT("Session registered successfully for user: %s"), *userId);

								LoginMenu->ChangeVisibilityForReLoginSession();

								ShowMainWidget();
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("Failed to register new session for user: %s"), *userId);
							}
						});
				}
			});
	}
}

void UEOSGameInstance::ShowWidget(TSubclassOf<UUserWidget> WidgetClass, UUserWidget*& Menu)
{
	if (CurrentMenu && CurrentMenu->IsInViewport())
	{
		PreviousMenu = CurrentMenu;
		CurrentMenu->RemoveFromParent();
	}

	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("WidgetClass is null!"));
		return;
	}

	Menu = CreateWidget<UUserWidget>(this, WidgetClass);
	if (!Menu)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create widget!"));
		return;
	}

	Menu->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!PlayerController)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerController is null!"));
		return;
	}

	FInputModeUIOnly InputModeData;
	TSharedRef<SWidget> WidgetToFocus = Menu->TakeWidget();

	if (WidgetToFocus->SupportsKeyboardFocus())
	{
		InputModeData.SetWidgetToFocus(WidgetToFocus);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("The widget '%s' does not support focus!"), *WidgetToFocus->GetTypeAsString());
	}

	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	CurrentMenu = Menu;
}

void UEOSGameInstance::ShowMainWidget()
{
	ShowWidget(MainMenuClass, reinterpret_cast<UUserWidget*&>(MainMenu));
}

void UEOSGameInstance::ShowLoginWidget()
{
	ShowWidget(LoginMenuClass, reinterpret_cast<UUserWidget*&>(LoginMenu));
}

void UEOSGameInstance::ShowLevelSelectWidget()
{
	ShowWidget(LevelSelectMenuClass, reinterpret_cast<UUserWidget*&>(LevelSelectMenu));
}

void UEOSGameInstance::ShowPauseWidget()
{
	ShowWidget(PauseMenuClass, reinterpret_cast<UUserWidget*&>(PauseMenu));
}

void UEOSGameInstance::ShowSettingsWidget()
{
	ShowWidget(SettingsMenuClass, reinterpret_cast<UUserWidget*&>(SettingsMenu));
}

void UEOSGameInstance::BackToPreviousWidget()
{
	ShowWidget(PreviousMenu->GetClass(), reinterpret_cast<UUserWidget*&>(PreviousMenu));
}

void UEOSGameInstance::WriteNewRecordToLeaderboard(float Point)
{
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	SessionManager = NewObject<USessionManager>();
	SessionManager->SetAccountId(userId);

	if (SessionManager)
	{
		SessionManager->AddRecord(userId, LevelName, Point, [this](bool bWasSuccessful)
			{
				if (bWasSuccessful)
				{
					UE_LOG(LogTemp, Log, TEXT("Adding was success"));
				}
				else
				{
					UE_LOG(LogTemp, Error, TEXT("Error to add"));
				}
			});
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("SessionManager dont initialize"));
	}
}

void UEOSGameInstance::ReadNewRecordToLeaderboard()
{
	SessionManager = NewObject<USessionManager>();
	SessionManager->SetAccountId(userId);

	SessionManager->GetAllLeaderboardRecords(
		[this](bool bSuccess, const TArray<FLeaderboardRecord>& LeaderboardRecords)
		{
			if (bSuccess)
			{
				FString LeaderboardText;
				FString PlayerRecordText;

				TArray<FLeaderboardRecord> FilteredRecords;

				FString llll = GetWorld()->GetCurrentLevel()->GetOuter()->GetName();

				for (const FLeaderboardRecord& Record : LeaderboardRecords)
				{
					if (Record.LevelName == GetWorld()->GetCurrentLevel()->GetOuter()->GetName())
					{
						FilteredRecords.Add(Record);

						if (Record.AccountId == userId)
						{
							PlayerRecordText = FString::Printf(TEXT("Your Record: %f"), Record.RecordMax);
						}
					}	
				}

				FilteredRecords.Sort([](const FLeaderboardRecord& A, const FLeaderboardRecord& B)
					{
						return A.RecordMax < B.RecordMax;
					});

				for (int32 i = 0; i < FMath::Min(5, FilteredRecords.Num()); i++)
				{
					const FLeaderboardRecord& Record = FilteredRecords[i];
					FString UserNickName = identityPtr->GetPlayerNickname(*identityPtr->CreateUniquePlayerId(Record.AccountId));
					LeaderboardText.Append(FString::Printf(TEXT("Account: %s, RecordMax: %f\n"), *UserNickName, Record.RecordMax));
				}

				ARecordBoard* RecordBoard = Cast<ARecordBoard>(UGameplayStatics::GetActorOfClass(GetWorld(), ARecordBoard::StaticClass()));

				if (RecordBoard)
				{
					RecordBoard->LeaderboardTextRender->SetText(FText::FromString(LeaderboardText));

					RecordBoard->PlayerTextRender->SetText(FText::FromString(PlayerRecordText));
				}
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("Failed to rettrieve leaderboard records."));
			}
		}
	);
}


FString UEOSGameInstance::GetNickName()
{
	return identityPtr->GetPlayerNickname(1);
}

void UEOSGameInstance::Init()
{
	Super::Init();

	onlineSubsystem = IOnlineSubsystem::Get();
	identityPtr = onlineSubsystem->GetIdentityInterface();
	identityPtr->OnLoginCompleteDelegates->AddUObject(this, &UEOSGameInstance::LoginCompleted);

	if (!SessionManager)
	{
		SessionManager = NewObject<USessionManager>();
	}
}

void UEOSGameInstance::Shutdown()
{
	if (identityPtr && isLogged)
	{
		identityPtr->Logout(0);
	}

	if (!isActiveAccount && isLogged)
	{
		UE_LOG(LogTemp, Warning, TEXT("Closing conn for user: %s"), *userId);

		SessionManager = NewObject<USessionManager>();

		SessionManager->LogoutSession(userId, [this](bool bIsActive)
			{
				if (bIsActive)
				{
					UE_LOG(LogTemp, Warning, TEXT("logout for user: %s"), *userId);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("logout failed for user: %s"), *userId);
				}
			});

		GetWorld()->GetTimerManager().ClearTimer(PingTimerHandle);
	}

	Super::Shutdown();
}

void UEOSGameInstance::PingServer()
{
	SessionManager = NewObject<USessionManager>();
	SessionManager->SetAccountId(this->userId);
	SessionManager->PingServer([this](bool isActive)
		{
			if (!isActive)
			{
				GetWorld()->GetTimerManager().ClearTimer(PingTimerHandle);
				UGameplayStatics::OpenLevel(this, FName("MainMenu_Level"));
			}
		});
}


void UEOSGameInstance::LoginCompleted(int numOfPlayers, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error)
{
	if (bWasSuccessful)
	{
		isLogged = true;
		this->userId = UserId.ToString();

		UE_LOG(LogTemp, Warning, TEXT("Checking account status for user: %s"), *userId);

		SessionManager = NewObject<USessionManager>();
		SessionManager->SetAccountId(this->userId);

		SessionManager->CheckAccountStatus(userId, [this](bool bIsActive)
			{
				if (bIsActive)
				{
					UE_LOG(LogTemp, Warning, TEXT("Account is already active for user: %s"), *userId);

					isActiveAccount = true;

					LoginMenu->ChangeVisibilityForReLoginSession();
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("Registering new session for user: %s"), *userId);

					SessionManager->RegisterNewSession(userId, [this](bool bSuccess)
						{
							if (bSuccess)
							{
								GetWorld()->GetTimerManager().SetTimer(PingTimerHandle, this, &UEOSGameInstance::PingServer, 10.0f, true);

								UE_LOG(LogTemp, Warning, TEXT("Session registered successfully for user: %s"), *userId);

								ShowMainWidget();
							}
							else
							{
								UE_LOG(LogTemp, Error, TEXT("Failed to register new session for user: %s"), *userId);
							}
						});
				}
			});
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Logged failed: %s"), *Error);
	}
}

//void UEOSGameInstance::LogoutCompleted(int numOfPlayers, bool bWasSuccessful)
//{
//	if (bWasSuccessful)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Checking account status for user: %s"), *userId);
//
//		if (!isActiveAccount)
//		{
//			SessionManager = NewObject<USessionManager>();
//			SessionManager->SetAccountId(this->userId);
//
//			SessionManager->LogoutSession(userId, [this](bool bIsActive)
//				{
//					if (bIsActive)
//					{
//						isLogged = false;
//
//						GetWorld()->GetTimerManager().ClearTimer(PingTimerHandle);
//
//						UE_LOG(LogTemp, Warning, TEXT("logout for user: %s"), *userId);
//					}
//					else
//					{
//						UE_LOG(LogTemp, Warning, TEXT("logout failed for user: %s"), *userId);
//					}
//				});
//		}
//		isActiveAccount = false;
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("Logout failed"));
//	}
//}