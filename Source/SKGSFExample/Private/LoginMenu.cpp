// Copyright 2023, Dakota Dawe, All rights reserved


#include "LoginMenu.h"
#include "EOSGameInstance.h"
#include "Components/Button.h"

void ULoginMenu::NativeConstruct()
{
	Super::NativeConstruct();

	gameInstance = GetGameInstance<UEOSGameInstance>();

	LoginBtn->OnClicked.AddDynamic(this, &ULoginMenu::LoginBtnClicked);
	ReLoginSessionTryBtn->OnClicked.AddDynamic(this, &ULoginMenu::ReLoginSessionTryBtnClicked);
}

void ULoginMenu::LoginBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->Login();
	}
}

void ULoginMenu::ChangeVisibilityForReLoginSession()
{
	ReLoginSessionTryBtn->SetVisibility(
		ReLoginSessionTryBtn->Visibility == ESlateVisibility::Visible ? ESlateVisibility::Hidden : ESlateVisibility::Visible
	);
}


void ULoginMenu::ReLoginSessionTryBtnClicked()
{
	if (gameInstance)
	{
		gameInstance->ReLoginSessionTry();
	}
}
