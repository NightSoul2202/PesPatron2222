// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenu.generated.h"

/**
 *
 */
UCLASS()
class SKGSFEXAMPLE_API UPauseMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstance* gameInstance;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* SettingsBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* MainMenuBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* NewRecordBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* GetRecordBtn;

	UFUNCTION()
	void SettingsBtnClicked();

	UFUNCTION()
	void MainMenuBtnClicked();

	UFUNCTION()
	void NewRecordBtnClicked();

	UFUNCTION()
	void GetRecordBtnClicked();

};
