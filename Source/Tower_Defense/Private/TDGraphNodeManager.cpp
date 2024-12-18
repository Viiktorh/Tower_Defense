// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGraphNodeManager.h"
#include "TDGraphNode.h"
#include "Enemy.h"
#include "TDPriorityQueue.h"

// Sets default values
ATDGraphNodeManager::ATDGraphNodeManager()
{

    PrimaryActorTick.bCanEverTick = false;

}

void ATDGraphNodeManager::BeginPlay()
{
    Super::BeginPlay();
    AStarSearch();
}

TArray<ATDGraphNode*> ATDGraphNodeManager::BreadthFirstSearch()
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

        //Once CurrentNode has found the EndNode, Add Nodes to the Path Array at index 0,
        //starting with the EndNode and using the Visited Map as a guide backwards towards the StartNode until we run out of nodes. 
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

        //Go to Neighbor if Visited-Map does not contain Neighbor-Key.
        //Add visited Neighbor to the Visited-Map with Current Node as value.
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

double ATDGraphNodeManager::Heuristic(ATDGraphNode* StartNodeLocation,ATDGraphNode* EndNodeLocation)
{
    double Heuristic =   StartNodeLocation->GetActorLocation().X - EndNodeLocation->GetActorLocation().X
					   + StartNodeLocation->GetActorLocation().Y - EndNodeLocation->GetActorLocation().Y;
    return abs(Heuristic);
}


TArray<ATDGraphNode*> ATDGraphNodeManager::AStarSearch()
{
	TMap<ATDGraphNode*, ATDGraphNode*> CameFrom;
	TMap<ATDGraphNode*, double> CostSoFar;
    TArray<ATDGraphNode*> CameFromKeyArray;
    TArray<ATDGraphNode*> Path;
    ATDGraphNode* Current = nullptr;
    //TPriorityQueue is not a Standard Unreal Class. See TDPriorityQueue.h
    TPriorityQueue<ATDGraphNode*> Frontier;
    

    if (!StartNode || !EndNode)
    {
        return Path;
    }
	Frontier.Push(StartNode, 0);

    CameFrom.FindOrAdd(StartNode) = StartNode;
    CostSoFar.FindOrAdd(StartNode) = 0;

    while (!Frontier.IsEmpty())
    {
        Current = Frontier.Pop();
        if (Current == EndNode)
        {
            break;
        }
		
        //Find the Path
        for (TObjectPtr<ATDGraphNode> Next : Current->Neighbors)
        {
        	double NewCost = CostSoFar.FindOrAdd(Current) + Current->CostToNeighbors.FindRef(Next);

            if ((!CostSoFar.Contains(Next) || NewCost < CostSoFar.FindOrAdd(Next)) 
                && !CameFrom.Contains(Next))
            {
                CostSoFar.FindOrAdd(Next) = NewCost;
                // Get Priority for next node
                double Priority = NewCost + Heuristic(Next, EndNode);

                //Push priority for Next node into the Frontier
                Frontier.Push(Next, Priority);
                CameFrom.FindOrAdd(Next) = Current;

                //Draw A red line for every node that has been searched. 
                DrawDebugLine(GetWorld(), Next->GetActorLocation(), Current->GetActorLocation(), FColor::Red, true, -1.0f, 2, 5.0f);
            }
        	
        }
    }

	CameFrom.GenerateKeyArray(CameFromKeyArray);
	if (CameFrom.FindRef(EndNode) != CameFrom.FindRef(CameFromKeyArray.Last()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 11.f, FColor::Blue, FString::Printf(TEXT("Could Not Find Any Path")));
		//No path can be found 
	}

    /*
     * Reconstruct the Path
     */
	// Retrace the path and put nodes into an Array,
	while (Current != StartNode)
	{
		Path.Emplace(Current);
		Current = CameFrom.FindOrAdd(Current);
	}
	    
    
    // Start Node does not get added to the Path in the Algorithm and needs to be emplaced manually. 
	// The path is initially the reverse order and has to be reversed again to make it correct.
    // Store the Path for later so you do not have to recalculate the path for every single enemy that is spawned. 
	Path.Emplace(StartNode);
	Algo::Reverse(Path);
    StoredPath = Path;
	return Path;
	/*
	 * End of Path Reconstruction
	 */
}







