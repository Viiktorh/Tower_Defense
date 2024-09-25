// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TDStackInterface.h"
#include "UObject/NoExportTypes.h"
#include "TDStack.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API UTDStack : public UObject, public ITDStackInterface
{
	GENERATED_BODY()

	virtual void Push_Implementation(const FString& Data) override;
	virtual FString Pop_Implementation() override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Queue")
	TArray<FString> StringBeholder;

	UFUNCTION(BlueprintCallable)
	void PrintStackBeholder();
};
