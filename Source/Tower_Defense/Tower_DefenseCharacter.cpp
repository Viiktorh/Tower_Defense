#include "Tower_DefenseCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "Tower.h"

ATower_DefenseCharacter::ATower_DefenseCharacter()
{

    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    GetCharacterMovement()->bOrientRotationToMovement = true;
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;
    CameraBoom->bUsePawnControlRotation = false;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = false;

    PrimaryActorTick.bCanEverTick = true;
}

void ATower_DefenseCharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (PreviewTower)
    {
        APlayerController* PlayerController = Cast<APlayerController>(GetController());
        if (PlayerController)
        {
            FHitResult HitResult;
            if (PlayerController->GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), true, HitResult))
            {
                FVector SpawnLocation = HitResult.Location;
                PreviewTower->SetActorLocation(SpawnLocation);
            }
        }
    }
}

void ATower_DefenseCharacter::BeginPlay()
{
    Super::BeginPlay();
}

void ATower_DefenseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }

    if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATower_DefenseCharacter::Move);
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATower_DefenseCharacter::Look);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
        EnhancedInputComponent->BindAction(IA_PlaceTowerAction, ETriggerEvent::Started, this, &ATower_DefenseCharacter::PlaceTower);
        EnhancedInputComponent->BindAction(IA_PlaceTowerAction, ETriggerEvent::Completed, this, &ATower_DefenseCharacter::FinalizeTowerPlacement);
        EnhancedInputComponent->BindAction(IA_UndoTowerAction, ETriggerEvent::Started, this, &ATower_DefenseCharacter::UndoTowerPlacement);
    }
}

void ATower_DefenseCharacter::PlaceTower()
{
    if (!TowerBlueprint) return;

    APlayerController* PlayerController = Cast<APlayerController>(GetController());
    if (!PlayerController) return;

    FVector CameraLocation;
    FRotator CameraRotation;
    PlayerController->GetPlayerViewPoint(CameraLocation, CameraRotation);
    FVector SpawnLocation = CameraLocation + CameraRotation.Vector() * 100.0f;

    PreviewTower = GetWorld()->SpawnActor<ATower>(TowerBlueprint, SpawnLocation, FRotator::ZeroRotator);

    if (PreviewTower)
    {
        PreviewTower->SetActorEnableCollision(false);
        PreviewTower->SetActorTickEnabled(false);

        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            PC->bShowMouseCursor = true;
            PC->bEnableClickEvents = true;
            PC->bEnableMouseOverEvents = true;
        }
    }
}

void ATower_DefenseCharacter::FinalizeTowerPlacement()
//**we are aware you can place the tower on yourself, we do not have valid locations checks currently**//
{
    if (PreviewTower)
    {
        PreviewTower->SetActorEnableCollision(true);
        PreviewTower->SetActorTickEnabled(true);
        PlacedTowers.Add(PreviewTower);
        PreviewTower = nullptr;

        if (APlayerController* PC = Cast<APlayerController>(GetController()))
        {
            PC->bShowMouseCursor = false;
            PC->bEnableClickEvents = false;
            PC->bEnableMouseOverEvents = false;
        }
    }
}

void ATower_DefenseCharacter::UndoTowerPlacement()
{
    if (PlacedTowers.Num() > 0)
    {
        ATower* LastTower = PlacedTowers.Pop();
        if (LastTower)
        {
            LastTower->Destroy();
        }
    }
}
