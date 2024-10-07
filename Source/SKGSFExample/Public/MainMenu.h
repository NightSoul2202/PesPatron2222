// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 *
 */
UCLASS()
class SKGSFEXAMPLE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstance* gameInstance;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* PlayBtn;

	UPROPERTY(meta = (BindWidget))

	class UButton* SettingsBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ExitBtn;

	UFUNCTION()
	void PlayBtnClicked();

	UFUNCTION()
	void SettingsBtnClicked();
	
};
