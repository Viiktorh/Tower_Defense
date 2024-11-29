// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tower_DefenseGameMode.h"
#include "Tower_DefenseCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Enemy.h"

ATower_DefenseGameMode::ATower_DefenseGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	PrimaryActorTick.bCanEverTick = true;
}

void ATower_DefenseGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	EnemyTimer -= DeltaTime;

	if (EnemyTimer < 0.0f)
	{
		EnemyTimer = 2.0f;

		UWorld* world = GetWorld();
		
		if (world && UGameplayStatics::GetPlayerCharacter(GetWorld(), 0) != nullptr)
		{
			int playerIndex = 0;

			FVector playerLocation = UGameplayStatics::GetPlayerCharacter(GetWorld(), playerIndex)->GetActorLocation();

			FVector enemyLocation = playerLocation;

			float randomDistance = FMath::RandRange(100.0f, 1000.0f);

			enemyLocation.X += randomDistance;
			enemyLocation.Y += randomDistance;

			//AEnemy* enemy = world->SpawnActor<AEnemy>(EnemyBlueprint, enemyLocation, FRotator::ZeroRotator);
		}
	}

}

void ATower_DefenseGameMode::BeginPlay()
{
	Super::BeginPlay();
}
