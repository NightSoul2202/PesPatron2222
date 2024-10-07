// Copyright 2023, Dakota Dawe, All rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RecordBoard.generated.h"

UCLASS()
class SKGSFEXAMPLE_API ARecordBoard : public AActor
{
	GENERATED_BODY()
	
public:	
	ARecordBoard();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* LeaderboardTextRender;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UTextRenderComponent* PlayerTextRender;


protected:
	virtual void BeginPlay() override;
	
};
