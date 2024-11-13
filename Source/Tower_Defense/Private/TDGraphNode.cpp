// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGraphNode.h"

// Sets default values
ATDGraphNode::ATDGraphNode()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

// Called when the game starts or when spawned
void ATDGraphNode::BeginPlay()
{
	Super::BeginPlay();

	// Draw lines to neighbor nodes
	for (ATDGraphNode* Neighbor : Neighbors)
	{
		if (Neighbor)
		{
			DrawDebugLine(
				GetWorld(),
				GetActorLocation(),
				Neighbor->GetActorLocation(),
				FColor::Yellow,
				true,
				-1.0f,
				0,
				5.0f
			);
			float Distance = GetDistanceTo(Neighbor);
			CostToNeighbors.Add(Distance);
		}
	}

}

void ATDGraphNode::AddNeighbor(ATDGraphNode* Neighbor)
{
	if (Neighbor && !Neighbors.Contains(Neighbor))
	{
		Neighbors.Add(Neighbor);
	}
}

double ATDGraphNode::GetCostToNeighbor(ATDGraphNode* CurrentNode, ATDGraphNode* Neighbor)
{
	return CurrentNode->GetDistanceTo(Neighbor);
}
