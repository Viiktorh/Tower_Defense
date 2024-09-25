// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CPPNodeObject.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API UCPPNodeObject : public UObject
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "NodeObject")
	UCPPNodeObject* Node(FString Data);

	/*
	 * Variables
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeObject")
	FString mData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NodeObject")
	TObjectPtr<UCPPNodeObject> NextNode;
};
