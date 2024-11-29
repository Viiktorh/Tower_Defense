// Fill out your copyright notice in the Description page of Project Settings.


#include "TDGraphNodeManager.h"
#include "TDGraphNode.h"
#include "Enemy.h"
#include "TDPriorityQueue.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

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
        Path = AStarSearch();
        if (Path.Num() > 0)
        {
            Enemy->SetPath(Path);
            Enemy->StartMoving();
        }
    }
}

TArray<ATDGraphNode*> ATDGraphNodeManager::FindShortestPath()
{
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
    double Heuristic = StartNodeLocation->GetActorLocation().X - EndNodeLocation->GetActorLocation().X + StartNodeLocation->GetActorLocation().Y - EndNodeLocation->GetActorLocation().Y;
    return abs(Heuristic);
    /*double VectorLength = StartNodeLocation->GetDistanceTo(EndNodeLocation);
	return (VectorLength);*/
}


TArray<ATDGraphNode*> ATDGraphNodeManager::AStarSearch()
{

	//TMap<ATDGraphNode*, ATDGraphNode*> CameFrom;
	//TMap<ATDGraphNode*, double> CostSoFar;
 //   TArray<ATDGraphNode*> CostSoFarKeyArray;
 //   TArray<ATDGraphNode*> CameFromKeyArray;
 //   TArray<ATDGraphNode*> Path;
 //   ATDGraphNode* Current;
 //   double NewCost;
    

    if (!StartNode || !EndNode)
    {
        return Path;
    }
    TMap<ATDGraphNode*, ATDGraphNode*> Visited;
    Visited.Add(StartNode, nullptr);

    TPriorityQueue<ATDGraphNode*> Frontier;
	Frontier.Push(StartNode, 0);


    CameFrom.FindOrAdd(StartNode) = StartNode;
    CostSoFar.FindOrAdd(StartNode) = 0;

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Checking Frontier...")));
    while (!Frontier.IsEmpty())
    {
        Current = Frontier.Pop();
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Emerald, FString::Printf(TEXT("Frontier Is not Empty")));
        if (Current == EndNode)
        {
            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Yellow, FString::Printf(TEXT("Current Node is EndNode")));
            break;
        }
		
        //Find the Path
        //TODO: Add A Visited-Check so Next does not revisit its own node. 
		
        for (TObjectPtr<ATDGraphNode> Next : Current->Neighbors)
        {
        	GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Emerald, FString::Printf(TEXT("Finding Path...")));
        	double NewCost = CostSoFar.FindOrAdd(Current) + Current->CostToNeighbors.FindRef(Next);
            
            CostSoFar.GenerateKeyArray(CostSoFarKeyArray);
            if ((!CostSoFar.Contains(Next) || NewCost < CostSoFar.FindOrAdd(Next)) 
                && !Visited.Contains(Next))
            {
            	Visited.Add(Next, Current);
                GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow, FString::Printf(TEXT("Adding Costs...")));
                CostSoFar.FindOrAdd(Next) = NewCost;
                // Get Priority for next node
                double Priority = NewCost + Heuristic(Next, EndNode);

                //Push priority for Next node into the Frontier
                Frontier.Push(Next, Priority);
                CameFrom.FindOrAdd(Next) = Current;

                DrawDebugLine(GetWorld(), Next->GetActorLocation(), Current->GetActorLocation(), FColor::Red, true, -1.0f, 2, 5.0f);
            }
        	
        }
    }

	CameFrom.GenerateKeyArray(CameFromKeyArray);
	if (CameFrom.FindRef(EndNode) != CameFrom.FindRef(CameFromKeyArray.Last()))
	{
		GEngine->AddOnScreenDebugMessage(-1, 11.f, FColor::Blue, FString::Printf(TEXT("Could Not Find Any Path"))); //No path can be found 
       
	}
	while (Current != StartNode)
	{
		//test this
		GEngine->AddOnScreenDebugMessage(-1, 11.f, FColor::Red, FString::Printf(TEXT("Adding New Elements to Path")));
		Path.Emplace(Current);
		Current = CameFrom.FindOrAdd(Current);
	}
	    
    
	//// Retrace the path and put into an Array
	Path.Emplace(StartNode);
	Algo::Reverse(Path);
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("Got to the end of the Code")));
	return Path;
	//return ReconstructPath(StartNode, EndNode, CameFrom);
}
//
//TArray<ATDGraphNode*> ATDGraphNodeManager::ReconstructPath(ATDGraphNode* Start, ATDGraphNode* Goal,
//	TMap<ATDGraphNode*, ATDGraphNode*> CameFrom)
//{
//	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Emerald, FString::Printf(TEXT("Attempting to Retrace Path")));
//    TArray<ATDGraphNode*> Path;
//    ATDGraphNode* CurrentNode = Goal;
//
//}






