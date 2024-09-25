// Fill out your copyright notice in the Description page of Project Settings.


#include "TDStack.h"

void UTDStack::Push_Implementation(const FString& Data)
{
	StringBeholder.Emplace(Data);
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Pushed: %s"), *Data));
}

FString UTDStack::Pop_Implementation()
{
	FString PopData = "";
	if (!StringBeholder.IsEmpty())
	{
		PopData = StringBeholder.Last();
		StringBeholder.Pop();
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Popped: %s"), *PopData));
	}
	return PopData;
}

void UTDStack::PrintStackBeholder()
{
	for (auto& Data : StringBeholder)
	{
		GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, FString(Data));
	}
}