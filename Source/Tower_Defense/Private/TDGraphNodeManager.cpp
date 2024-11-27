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
        TArray<ATDGraphNode*> Path = AStarSearch();
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

double ATDGraphNodeManager::Heuristic(ATDGraphNode* StartNodeLocation,ATDGraphNode* EndNodeLocation)
{
    /*if (StartNode != nullptr && EndNode != nullptr)
    {*/
    double VectorLength = StartNodeLocation->GetDistanceTo(EndNodeLocation);
	return VectorLength; 
    /*}*/
    //
    /*return 999999;*/
}



/*
 *Things to Check:
 *Is the Priority Queue Wrong? Does it need a .get() function?
 *Is the If-Statement Checking for CostSoFar Wrong? 
 */

TArray<ATDGraphNode*> ATDGraphNodeManager::AStarSearch()
{
    TMap<ATDGraphNode*, ATDGraphNode*> CameFrom;
    TMap<ATDGraphNode*, double> CostSoFar;
    TArray<ATDGraphNode*> CostSoFarKeyArray;
    TArray<ATDGraphNode*> CameFromKeyArray;
    TArray<ATDGraphNode*> Path;
    ATDGraphNode* Current;
    double NewCost;

    if (!StartNode || !EndNode)
    {
        return Path;
    }
    
    TPriorityQueue<ATDGraphNode*> Frontier;
	Frontier.Push(StartNode, 0);
   
    

    CameFrom.Emplace(StartNode, StartNode);
    CostSoFar.Emplace(StartNode, 0);

    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Emerald, FString::Printf(TEXT("Checking Frontier...")));
    while (!Frontier.IsEmpty())
    {
        Current = Frontier.Pop();
        GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Emerald, FString::Printf(TEXT("Frontier Is not Empty")));
        if (Current == EndNode)
        {
            GEngine->AddOnScreenDebugMessage(-1, 7.f, FColor::Emerald, FString::Printf(TEXT("Current Node is EndNode")));
            break;
        }

        
        //Find the Path
        for (ATDGraphNode* Next : Current->Neighbors)
        {
            GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Emerald, FString::Printf(TEXT("Finding Path...")));
        	NewCost = CostSoFar.FindOrAdd(Current) + Current->GetCostToNeighbor(Current, Next);
            /*
             *
             * Test this if Statement
             */
            //UE_LOG(LogTemp, Warning, TEXT("Current Node is %s" ),Current->GetActorLabel());
            GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("NewCost is %d, Cost so Far: %d"), NewCost, CostSoFar.FindRef(Next)));
            CostSoFar.GenerateKeyArray(CostSoFarKeyArray);
            if (CostSoFar.FindRef(Next) == CostSoFar[CostSoFarKeyArray.Last()] || NewCost < CostSoFar.FindOrAdd(Next))
            {
                GEngine->AddOnScreenDebugMessage(-1, 8.f, FColor::Yellow, FString::Printf(TEXT("Adding Costs...")));
                CostSoFar.FindOrAdd(Next) = NewCost;
                // Get Priority for next node
                double Priority = NewCost + Heuristic(Next, EndNode);

                //Push priority for Next node into the Frontier
                Frontier.Push(Next, Priority); //TODO: Check if Correct
                CameFrom.FindOrAdd(Next) = Current;

                GEngine->AddOnScreenDebugMessage(-1, 20.f, FColor::Emerald, FString::Printf(TEXT("NewCost is (in if-Statement) %d Cost So Far: %d"), NewCost, CostSoFar.FindRef(Next)));

                DrawDebugLine(GetWorld(), Next->GetActorLocation(), Current->GetActorLocation(), FColor::Red, true, -1.0f, 2, 5.0f);
            }
        	CostSoFarKeyArray.Empty();
        }
	    CameFrom.GenerateKeyArray(CameFromKeyArray);
		if (CameFrom.Find(EndNode) == CameFrom.Find(CameFromKeyArray.Last()))
		{
			GEngine->AddOnScreenDebugMessage(-1, 11.f, FColor::Blue, FString::Printf(TEXT("Could Not Find Any Path")));
			return Path; //No path can be found 
		}
		if (CameFrom.Contains(StartNode))
		{
			while (Current != StartNode)
			{
				//test this
				GEngine->AddOnScreenDebugMessage(-1, 11.f, FColor::Red, FString::Printf(TEXT("Adding New Elements to Path")));
				Path.Emplace(Current);
				Current = CameFrom[Current];
			}
	    }
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






