#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy.h"

AProjectile::AProjectile()
{
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;
    MeshComponent->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);

    ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
    ProjectileMovement->InitialSpeed = 1000.0f;
    ProjectileMovement->MaxSpeed = 1000.0f;
    ProjectileMovement->ProjectileGravityScale = 0.1f;
}

void AProjectile::BeginPlay()
{
    Super::BeginPlay();
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
    {
        Enemy->ApplyDamage(Damage);
        Destroy(); // Destroy the projectile upon hitting the enemy
    }
}
