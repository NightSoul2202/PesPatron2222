// Copyright 2023, Dakota Dawe, All rights reserved


#include "RecordBoard.h"
#include "EOSGameInstance.h"
#include "Components/TextRenderComponent.h"

ARecordBoard::ARecordBoard()
{
	PrimaryActorTick.bCanEverTick = true;
	LeaderboardTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("LeaderboardText"));
	LeaderboardTextRender->SetHorizontalAlignment(EHTA_Center);

	PlayerTextRender = CreateDefaultSubobject<UTextRenderComponent>(TEXT("PlayerLeaderboardText"));
	PlayerTextRender->SetHorizontalAlignment(EHTA_Center);
}

void ARecordBoard::BeginPlay()
{
	Super::BeginPlay();
	UEOSGameInstance* gameInstance = GetGameInstance<UEOSGameInstance>();

	gameInstance->ReadNewRecordToLeaderboard();
}

void ARecordBoard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

