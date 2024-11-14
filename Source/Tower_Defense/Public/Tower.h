#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tower.generated.h"

// Forward declarations
class AProjectile;
class UStaticMeshComponent;
class USphereComponent;

UCLASS()
class TOWER_DEFENSE_API ATower : public AActor
{
    GENERATED_BODY()

public:
    ATower();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

    // Components
    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* TowerMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* RangeSphere;

    // Firing Properties
    UPROPERTY(EditAnywhere, Category = "Firing")
    TSubclassOf<AProjectile> ProjectileClass;

    UPROPERTY(EditAnywhere, Category = "Firing")
    float FireRate = 2.0f;

private:
    // Keeps track of the time since the last shot was fired
    float TimeSinceLastShot;

    // Array to store enemies detected within range
    TArray<AActor*> EnemiesInRange;

    // Function to handle enemy detection within the tower's range
    UFUNCTION()
    void OnEnemyDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Function to spawn and shoot a projectile at the target
    void ShootProjectile(AActor* Target);
};
