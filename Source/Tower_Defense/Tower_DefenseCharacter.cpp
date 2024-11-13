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
    CameraBoom->bUsePawnControlRotation = true;

    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    FollowCamera->bUsePawnControlRotation = true;

    ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));
    ViewCamera->SetupAttachment(CameraBoom);


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
        Super::SetupPlayerInputComponent(PlayerInputComponent);
        PlayerInputComponent->BindAxis(FName("MoveForward"), this, &ATower_DefenseCharacter::MoveForward);
        PlayerInputComponent->BindAxis(FName("MoveRight"), this, &ATower_DefenseCharacter::MoveRight);
        PlayerInputComponent->BindAxis(FName("Turn"), this, &ATower_DefenseCharacter::Turn);
        PlayerInputComponent->BindAxis(FName("LookUp"), this, &ATower_DefenseCharacter::LookUp);

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

void ATower_DefenseCharacter::Move(const FInputActionValue& Value)
{
       // Ensure the input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    // Forward/backward movement (X-axis)
    if (MovementVector.Y != 0.f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
        AddMovementInput(Direction, MovementVector.Y);
    }

    // Right/left movement (Y-axis)
    if (MovementVector.X != 0.f)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
        AddMovementInput(Direction, MovementVector.X);
    }
}

void ATower_DefenseCharacter::Look(const FInputActionValue& Value)
{
    FVector2D LookValue = Value.Get<FVector2D>();

    // Add yaw input (mouse X-axis movement)
    AddControllerYawInput(LookValue.X);

    // Add pitch input (mouse Y-axis movement)
    AddControllerPitchInput(LookValue.Y);
}

void ATower_DefenseCharacter::PushEnemy()
{
//{
//    FHitResult OutHit;
//
//    AController* PlayerController = GetController();
//    //Determines where the Beam start and end locations are.
//    FRotator BeamRotation = FollowCamera->GetRelativeRotation();
//    FVector BeamStart = GetActorLocation();
//    FVector ForwardVector = FollowCamera->GetForwardVector();
//    FVector BeamEnd = ((ForwardVector * 1800.f) + BeamStart);
//    FVector PushBackDirection = BeamStart - (BeamStart.Z - 50.f);
//
//    //Initialize Collision parameters
//    FCollisionQueryParams CollisionParams;
//
//    DrawDebugLine(GetWorld(), BeamStart, BeamEnd, FColor::Red, false, 3.0f, 0, 1);
//
//    //Execute a LineTrace with OutHit as result
//    GetWorld()->LineTraceSingleByChannel(OutHit, BeamStart, BeamEnd, TraceChannelProperty, CollisionParams);
//
//    // If the trace hit something, bBlockingHit will be true,
//    // and its fields will be filled with detailed info about what was hit
//    if (OutHit.bBlockingHit && IsValid(OutHit.GetActor()))
//    {
//        if (OutHit.GetActor() != nullptr)
//        {
//            //TODO: This does not push enemies away, it only teleports them up.
//            FVector VectorToEnemy = GetActorLocation() + (OutHit.GetActor()->GetActorLocation() - GetActorLocation());
//            FVector PushBackEndPoint = VectorToEnemy + (GetActorLocation().ZAxisVector * 200.f);
//            OutHit.GetActor()->SetActorLocation(PushBackEndPoint);
//            //OutHit.GetComponent()->AddImpulse(ForwardVector*1000,FName("NAME_None"), true);
//            /*UPrimitiveComponent* component = Cast<UPrimitiveComponent>(OutHit.GetActor()->GetRootComponent());
//            if (component != nullptr)
//            {
//                component->AddImpulseAtLocation(ForwardVector*500, BeamEnd);
//            }
//            OutHit.GetActor()->GetRootComponent()->AddLocalRotation(BeamRotation);*/
//            //SetActorLocation(GetActorLocation() + (Direction * MoveSpeed * GetWorld()->GetDeltaSeconds()));
//            DrawDebugLine(GetWorld(), BeamStart, PushBackEndPoint, FColor::Cyan, false, 2.0f, 0, 1);
//        }
//    }
//}
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
void ATower_DefenseCharacter::MoveForward(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
        AddMovementInput(Direction, Value);
    }
}

void ATower_DefenseCharacter::MoveRight(float Value)
{
    if ((Controller != NULL) && (Value != 0.0f))
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
        AddMovementInput(Direction, Value);
    }
}

void ATower_DefenseCharacter::Turn(float Value)
{
    AddControllerYawInput(Value);
}

void ATower_DefenseCharacter::LookUp(float Value)
{
    AddControllerPitchInput(Value);
}