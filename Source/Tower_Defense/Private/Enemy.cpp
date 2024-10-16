#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    Health = 100;
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (MoveTarget)
    {
        MoveTowardsTarget(MoveTarget);
    }
}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ApplyDamage(10);
}

void AEnemy::ApplyDamage(int Damage)
{
    Health -= Damage;

    if (Health <= 0)
    {
        DestroyEnemy();
    }
}

void AEnemy::DestroyEnemy()
{
    Destroy();
}

void AEnemy::MoveTowardsTarget(AActor* Target)
{
    if (Target)
    {
        FVector Direction = (Target->GetActorLocation() - GetActorLocation()).GetSafeNormal();
        SetActorLocation(GetActorLocation() + (Direction * MoveSpeed * GetWorld()->GetDeltaSeconds()));
    }
}
