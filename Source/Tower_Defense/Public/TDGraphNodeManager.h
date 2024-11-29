// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDPriorityQueue.h"
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


	UFUNCTION(BlueprintCallable, Category = "GraphManager")
	TArray<ATDGraphNode*> AStarSearch();

	TArray<ATDGraphNode*> ReconstructPath(ATDGraphNode* Start, ATDGraphNode* Goal, TMap<ATDGraphNode*, ATDGraphNode*> CameFrom);

	/*
	 * Variables
	 */
	double Heuristic(ATDGraphNode* StartNodeLocation, ATDGraphNode* EndNodeLocation);

	//FVector StartNodeLocation;
	//FVector EndNodeLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	TArray<ATDGraphNode*> AllNodes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	ATDGraphNode* StartNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	ATDGraphNode* EndNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	AEnemy* Enemy;

	TMap<ATDGraphNode*, ATDGraphNode*> CameFrom;
	TMap<ATDGraphNode*, double> CostSoFar;
	TArray<ATDGraphNode*> CostSoFarKeyArray;
	TArray<ATDGraphNode*> CameFromKeyArray;
	TArray<ATDGraphNode*> Path;
	ATDGraphNode* Current;

protected:
	virtual void BeginPlay() override;
};
