// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "EOSGameInstance.h"
#include "Components/Button.h"

void UMainMenu::NativeConstruct()
{
	Super::NativeConstruct();

	gameInstance = GetGameInstance<UEOSGameInstance>();

	PlayBtn->OnClicked.AddDynamic(this, &UMainMenu::PlayBtnClicked);
	SettingsBtn->OnClicked.AddDynamic(this, &UMainMenu::SettingsBtnClicked);
}

void UMainMenu::PlayBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->ShowLevelSelectWidget();
	}
}

void UMainMenu::SettingsBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->ShowSettingsWidget();
	}
}
