// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDQueueInterface.h"
#include "TDStackInterface.h"
#include "UObject/NoExportTypes.h"
#include "TDArrayObject.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API UTDArrayObject : public UObject, public ITDQueueInterface, public ITDStackInterface
{
	GENERATED_BODY()

	// Start Interface
	virtual void Enqueue_Implementation(int32 Data) override;
	virtual int32 Dequeue_Implementation() override;
	virtual void IntPush_Implementation(const int32 Data) override;
	virtual int32 IntPop_Implementation() override;
	// End Interface

public:

	UFUNCTION(BlueprintCallable)
	void Print();

	UFUNCTION(BlueprintCallable)
	int32 FibNumber(int n);

	/*
	 * Variables
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> IntArray;
};
