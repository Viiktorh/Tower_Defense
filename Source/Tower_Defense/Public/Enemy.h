#pragma once

#include "CoreMinimal.h"
#include "TDGraphNodeManager.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Enemy.generated.h"

class ATDGraphNode;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class TOWER_DEFENSE_API AEnemy : public ACharacter
{
    GENERATED_BODY()

public:
    AEnemy();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;
    void ApplyDamage(int Damage);
    void DestroyEnemy();
    void MoveTowardsTarget(AActor* Target);

    UFUNCTION()
    void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

    UFUNCTION(BlueprintCallable)
    void SetPath(const TArray<ATDGraphNode*> NewPath);

    UFUNCTION()
    void StartMoving();

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USkeletalMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditAnywhere, Category = "Health")
    int Health;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    AActor* MoveTarget = nullptr;

    UPROPERTY()
    TArray<ATDGraphNode*> PathNodes;

    UPROPERTY()
    TArray<ATDGraphNodeManager*> AllNodes;

    UPROPERTY(VisibleAnywhere, Category = "PathCharacter")
    int32 CurrentNodeIndex;

    UPROPERTY(VisibleAnywhere, Category = "PathCharacter")
    bool bIsMoving;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UWidgetComponent* HealthBarWidgetComponent;
};
