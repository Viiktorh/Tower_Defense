// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tower_DefenseGameMode.generated.h"

UCLASS(minimalapi)
class ATower_DefenseGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATower_DefenseGameMode();

	UPROPERTY(EditAnywhere, Category = "Spawn")
	TSubclassOf<class AEnemy> EnemyBlueprint;

	float EnemyTimer;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;
};



