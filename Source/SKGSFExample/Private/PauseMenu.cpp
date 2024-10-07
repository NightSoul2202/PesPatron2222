// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "EOSGameInstance.h"
#include "Components/Button.h"
#include <Kismet/GameplayStatics.h>

void UPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	gameInstance = GetGameInstance<UEOSGameInstance>();

	SettingsBtn->OnClicked.AddDynamic(this, &UPauseMenu::SettingsBtnClicked);
	MainMenuBtn->OnClicked.AddDynamic(this, &UPauseMenu::MainMenuBtnClicked);
	NewRecordBtn->OnClicked.AddDynamic(this, &UPauseMenu::NewRecordBtnClicked);
	GetRecordBtn->OnClicked.AddDynamic(this, &UPauseMenu::GetRecordBtnClicked);
}

void UPauseMenu::SettingsBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->ShowSettingsWidget();
	}
}

void UPauseMenu::MainMenuBtnClicked()
{
	UGameplayStatics::OpenLevel(this, FName("MainMenu"));
}

void UPauseMenu::NewRecordBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->WriteNewRecordToLeaderboard(10);
	}
}

void UPauseMenu::GetRecordBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->ReadNewRecordToLeaderboard();
	}
}
