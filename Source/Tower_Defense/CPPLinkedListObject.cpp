// Fill out your copyright notice in the Description page of Project Settings.


#include "CPPLinkedListObject.h"

#include "CppNodeObject.h"

void UCPPLinkedListObject::Append(FString Data)
{
	mData = Data;
	UCPPNodeObject* NewNode = NewObject<UCPPNodeObject>(GetTransientPackage());
	NewNode->Node(mData);

	if (IsValid(Head))
	{
		CurrentNode = Head;
		while (IsValid(CurrentNode->NextNode))
		{
			CurrentNode = CurrentNode->NextNode;
		}
		CurrentNode->NextNode = NewNode;
	}
	else
	{
		Head = NewNode;
	}
}

void UCPPLinkedListObject::Prepend(FString Data)
{
	mData = Data;
	UCPPNodeObject* NewNode = NewObject<UCPPNodeObject>(GetTransientPackage());
	NewNode->Node(mData);

	if (IsValid(Head))
	{
		CurrentNode = Head;
	}
}

void UCPPLinkedListObject::DeleteWithValue()
{
}

void UCPPLinkedListObject::PrintList()
{
}

//
//FString ULinkedListObject::PrintList()
//{
//	return Data;
//}