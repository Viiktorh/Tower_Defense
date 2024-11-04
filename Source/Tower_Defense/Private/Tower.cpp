#include "Tower.h"
#include "Projectile.h"
#include "Enemy.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

ATower::ATower()
{
    PrimaryActorTick.bCanEverTick = true;

    // Initialize TowerMesh
    TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
    RootComponent = TowerMesh;

    // Initialize RangeSphere
    RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
    RangeSphere->SetupAttachment(RootComponent);
    RangeSphere->SetSphereRadius(1000.0f); // Set the detection range
    RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &ATower::OnEnemyDetected);

    // Initialize fire rate and time tracking
    TimeSinceLastShot = 0.0f;
}

void ATower::BeginPlay()
{
    Super::BeginPlay();
}

void ATower::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    TimeSinceLastShot += DeltaTime;

    if (TimeSinceLastShot >= FireRate && EnemiesInRange.Num() > 0)
    {
        AActor* TargetEnemy = EnemiesInRange[0]; // Target the first enemy in range
        ShootProjectile(TargetEnemy);
        TimeSinceLastShot = 0.0f;
    }
}

void ATower::OnEnemyDetected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (AEnemy* Enemy = Cast<AEnemy>(OtherActor))
    {
        EnemiesInRange.Add(Enemy);
    }
}

void ATower::ShootProjectile(AActor* Target)
{
    FVector TossVelocity;
    const FVector Start = GetActorLocation();
    const FVector End = Target->GetActorLocation();
    float TossSpeed = 2000.0f; // Adjust this to match your needs
    bool bFound = UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, Start, End, TossSpeed, false, 0.0f, 0.0f, ESuggestProjVelocityTraceOption::DoNotTrace);
    if (bFound)
    {
        FRotator ProjectileRotation = TossVelocity.Rotation();
        AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, Start, ProjectileRotation);
        if (Projectile)
        {
            Projectile->ProjectileMovement->Velocity = TossVelocity;
        }
    }
}
