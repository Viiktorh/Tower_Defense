#include "Enemy.h"
#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UEnemyHealthBar.h"
#include "Kismet/GameplayStatics.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
    RootComponent = BoxComponent;
    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    Health = 100;

    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
    HealthBarWidgetComponent->SetupAttachment(RootComponent);
    HealthBarWidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
    HealthBarWidgetComponent->SetRelativeLocation(FVector(0, 0, 100));
}

void AEnemy::BeginPlay()
{
    Super::BeginPlay();

    UUserWidget* UserWidget = HealthBarWidgetComponent->GetUserWidgetObject();
    UUEnemyHealthBar* EnemyHealthBar = Cast<UUEnemyHealthBar>(UserWidget);
    if (EnemyHealthBar)
    {
        EnemyHealthBar->Health = static_cast<float>(Health) / 100.0f;
    }
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

    // Update health bar
    UUserWidget* UserWidget = HealthBarWidgetComponent->GetUserWidgetObject();
    UUEnemyHealthBar* EnemyHealthBar = Cast<UUEnemyHealthBar>(UserWidget);
    if (EnemyHealthBar)
    {
        EnemyHealthBar->Health = static_cast<float>(Health) / 100.0f; // Assuming 100 is the max health
    }

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
