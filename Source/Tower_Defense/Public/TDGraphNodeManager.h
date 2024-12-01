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
	TArray<ATDGraphNode*> BreadthFirstSearch();


	UFUNCTION(BlueprintCallable, Category = "GraphManager")
	TArray<ATDGraphNode*> AStarSearch();

	double Heuristic(ATDGraphNode* StartNodeLocation, ATDGraphNode* EndNodeLocation);

	/*
	 * Variables
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	ATDGraphNode* StartNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	ATDGraphNode* EndNode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphManager")
	AEnemy* Enemy;

	TArray<ATDGraphNode*> StoredPath;

protected:
	virtual void BeginPlay() override;
};
