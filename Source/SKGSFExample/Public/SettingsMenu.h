// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingsMenu.generated.h"

/**
 *
 */
UCLASS()
class SKGSFEXAMPLE_API USettingsMenu : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstance* gameInstance;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* BackBtn;

	UFUNCTION()
	void BackBtnClicked();
};
