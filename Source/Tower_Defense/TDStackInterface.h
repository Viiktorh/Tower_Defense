// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TDStackInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UTDStackInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOWER_DEFENSE_API ITDStackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//FString 
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackInterface")
	void Push(const FString& Data);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackInterface")
	FString Pop();

	//Int32
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackInterface")
	void IntPush(const int32 Data);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "StackInterface")
	int32 IntPop();
};
