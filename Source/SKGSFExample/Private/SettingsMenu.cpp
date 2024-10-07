// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsMenu.h"
#include "EOSGameInstance.h"
#include "Components/Button.h"

void USettingsMenu::NativeConstruct()
{
	Super::NativeConstruct();

	gameInstance = GetGameInstance<UEOSGameInstance>();

	BackBtn->OnClicked.AddDynamic(this, &USettingsMenu::BackBtnClicked);
}

void USettingsMenu::BackBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->BackToPreviousWidget();
	}
}
