// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDGraphNodeManager.generated.h"

class AEnemy;
class ATDGraphNode;

UCLASS()
class TOWER_DEFENSE_API ATDGraphNodeManager : public AActor
{
	GENERATED_BODY()

public:
	ATDGraphNodeManager();

	UFUNCTION(BlueprintCallable, Category = "GraphManager")
	TArray<ATDGraphNode*> FindShortestPath();

	/*
	 * Variables
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	TArray<ATDGraphNode*> AllNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	ATDGraphNode* StartNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	ATDGraphNode* EndNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	AEnemy* Enemy;

protected:
	virtual void BeginPlay() override;
};
