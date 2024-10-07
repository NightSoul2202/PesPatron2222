// Copyright 2023, Dakota Dawe, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoginMenu.generated.h"

UCLASS()
class SKGSFEXAMPLE_API ULoginMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	void ChangeVisibilityForReLoginSession();

protected:
	virtual void NativeConstruct() override;

	class UEOSGameInstance* gameInstance;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* LoginBtn;

	UPROPERTY(meta = (BindWidget))
	class UButton* ReLoginSessionTryBtn;

	UFUNCTION()
	void LoginBtnClicked();

	UFUNCTION()
	void ReLoginSessionTryBtnClicked();
	
};
