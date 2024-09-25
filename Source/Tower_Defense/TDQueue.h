// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDQueueInterface.h"
#include "UObject/NoExportTypes.h"
#include "TDQueue.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API UTDQueue : public UObject, public ITDQueueInterface
{
	GENERATED_BODY()

public:
	//CppQueueInterface Begin
	virtual void Enqueue_Implementation(int32 Data) override;

	virtual int32 Dequeue_Implementation() override;
	//CppQueueInterface End

	/*
	 * Variable
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
	TArray<int32> IntBeholder;

	UFUNCTION(BlueprintCallable)
	void PrintIntBeholder();
};
