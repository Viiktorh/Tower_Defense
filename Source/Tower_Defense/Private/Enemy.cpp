#include "Enemy.h"
#include "TDGraphNode.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UEnemyHealthBar.h"
#include "Kismet/GameplayStatics.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"

AEnemy::AEnemy()
{
    PrimaryActorTick.bCanEverTick = true;
    RootComponent = MeshComponent;
    MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComponent"));
    MeshComponent->SetupAttachment(RootComponent);
    Health = 100;

    // AI movement
    CurrentNodeIndex = 0;
    bIsMoving = false;

    // Health Bar
    HealthBarWidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidgetComponent"));
    HealthBarWidgetComponent->SetupAttachment(MeshComponent);
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

    HealthBarWidgetComponent->SetVisibility(true);
}

void AEnemy::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (bIsMoving && PathNodes.Num() > 0 && CurrentNodeIndex < PathNodes.Num())
    {
        FVector TargetLocation = PathNodes[CurrentNodeIndex]->GetActorLocation();
        FVector Direction = (TargetLocation - GetActorLocation()).GetSafeNormal();
        FVector NewLocation = GetActorLocation() + Direction * MoveSpeed * DeltaTime;

        SetActorLocation(NewLocation);

        if (FVector::Dist(GetActorLocation(), TargetLocation) < 100.0f)
        {
            CurrentNodeIndex++;

            if (CurrentNodeIndex >= PathNodes.Num())
            {
                bIsMoving = false;
            }
        }
    }
}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    ApplyDamage(10);
}

void AEnemy::SetPath(const TArray<ATDGraphNode*> NewPath)
{
    PathNodes = NewPath;
    CurrentNodeIndex = 0;
}

void AEnemy::StartMoving()
{
    bIsMoving = true;
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
