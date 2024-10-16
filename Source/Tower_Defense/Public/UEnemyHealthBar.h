// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UEnemyHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class TOWER_DEFENSE_API UUEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health;
};
