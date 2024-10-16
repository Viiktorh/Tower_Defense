#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/WidgetComponent.h"
#include "Enemy.generated.h"

class UBoxComponent;
class UProjectileMovementComponent;
class UStaticMeshComponent;

UCLASS()
class TOWER_DEFENSE_API AEnemy : public AActor
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

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UBoxComponent* BoxComponent;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(VisibleAnywhere, Category = "Movement")
    UProjectileMovementComponent* MovementComponent;

    UPROPERTY(EditAnywhere, Category = "Health")
    int Health;

    UPROPERTY(EditAnywhere, Category = "Movement")
    float MoveSpeed = 200.0f;

    UPROPERTY(EditAnywhere, Category = "Movement")
    AActor* MoveTarget = nullptr;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UWidgetComponent* HealthBarWidgetComponent;
};
