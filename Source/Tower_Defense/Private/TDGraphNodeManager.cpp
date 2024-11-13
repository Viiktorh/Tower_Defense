// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGraphNodeManager.h"
#include "TDGraphNode.h"
#include "Enemy.h"
#include "TDPriorityQueue.h"

// Sets default values
ATDGraphNodeManager::ATDGraphNodeManager()
{
    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;

}

void ATDGraphNodeManager::BeginPlay()
{
    Super::BeginPlay();

    if (Enemy)
    {
        TArray<ATDGraphNode*> Path = FindShortestPath();
        if (Path.Num() > 0)
        {
            Enemy->SetPath(Path);
            Enemy->StartMoving();
        }
    }
}

TArray<ATDGraphNode*> ATDGraphNodeManager::FindShortestPath()
{
    TArray<ATDGraphNode*> Path;

    if (!StartNode || !EndNode)
    {
        return Path;
    }

    TQueue<ATDGraphNode*> Queue;
    TMap<ATDGraphNode*, ATDGraphNode*> Visited;

    Queue.Enqueue(StartNode);
    Visited.Add(StartNode, nullptr);

    while (!Queue.IsEmpty())
    {
        ATDGraphNode* CurrentNode;
        Queue.Dequeue(CurrentNode);

        if (CurrentNode == EndNode)
        {
            ATDGraphNode* Node = EndNode;
            while (Node)
            {
                Path.Insert(Node, 0);
                Node = Visited[Node];
            }
            break;
        }

        for (ATDGraphNode* Neighbor : CurrentNode->Neighbors)
        {
            if (!Visited.Contains(Neighbor))
            {
                Queue.Enqueue(Neighbor);
                Visited.Add(Neighbor, CurrentNode);
            }
        }
    }

    return Path;
}

float ATDGraphNodeManager::Heuristic(FVector StartNodeLocation,FVector EndNodeLocation)
{
    if (StartNode != nullptr && EndNode != nullptr)
    {
    	StartNodeLocation = StartNode->GetActorLocation();
    	EndNodeLocation = EndNode->GetActorLocation();
    	return (StartNodeLocation.X - EndNodeLocation.X) + (StartNodeLocation.Y - EndNodeLocation.Y); 
    }
    return 0;
}

TArray<ATDGraphNode*> ATDGraphNodeManager::AStarSearch()
{
    TMap<ATDGraphNode*, ATDGraphNode*> CameFrom;
    TMap<ATDGraphNode*, double> CostSoFar;
    TArray<ATDGraphNode*> Path;
    if (!StartNode || !EndNode)
    {
        return Path;
    }

    TPriorityQueue<ATDGraphNode*> Frontier;
	Frontier.Push(StartNode, 0);

    CameFrom.Add(StartNode, StartNode);
    CostSoFar.Add(StartNode, 0);
    
    while (!Frontier.IsEmpty())
    {
        ATDGraphNode* Current = Frontier.Pop();

        if (Current == EndNode)
        {
            break;
        }

        for(ATDGraphNode* Next : Current->Neighbors)
        {
            double NewCost = CostSoFar[Current] + Current->GetCostToNeighbor(Current, Next);
            if (CostSoFar.FindRef(Next) == CostSoFar.Num() 
                || NewCost < CostSoFar.FindRef(Next))
            {
                CostSoFar[Next] = NewCost;
                double Priority = NewCost + Heuristic(Next->GetActorLocation(), EndNode->GetActorLocation());
                Frontier.Push(Next, Priority);
                CameFrom[Next] = Current;
            }
        }
    }
}

