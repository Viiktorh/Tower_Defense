// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TDGraphNode.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API ATDGraphNode : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATDGraphNode();

	void AddNeighbor(ATDGraphNode* Neighbor);

	/*
	 * Variables
	 */
	UPROPERTY(VisibleAnywhere, Category = "GraphNode")
	class UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GraphNode")
	TArray<ATDGraphNode*> Neighbors;

	UPROPERTY(BlueprintReadOnly, Category = "GraphNode")
	TArray<float> DistanceToNeighbors;

protected:
	virtual void BeginPlay() override;
};
