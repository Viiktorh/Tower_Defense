// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPPLinkedListObject.generated.h"

class UCPPNodeObject;
/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API UCPPLinkedListObject : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "LinkedList")
	void Append(FString Data);

	UFUNCTION(BlueprintCallable, Category = "LinkedList")
	void Prepend(FString Data);

	UFUNCTION(BlueprintCallable, Category = "LinkedList")
	void DeleteWithValue();

	UFUNCTION(BlueprintCallable, Category = "LinkedList")
	void PrintList();

	/*
	 * Variables
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedList")
	TObjectPtr<UCPPNodeObject> Head;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedList")
	FString mData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LinkedList")
	TObjectPtr<UCPPNodeObject> CurrentNode;
};
