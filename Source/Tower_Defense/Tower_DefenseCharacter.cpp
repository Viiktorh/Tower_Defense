// Copyright Epic Games, Inc. All Rights Reserved.

#include "Tower_DefenseCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "DrawDebugHelpers.h"
#include "Enemy.h"


//////////////////////////////////////////////////////////////////////////
// ATower_DefenseCharacter

ATower_DefenseCharacter::ATower_DefenseCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ATower_DefenseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void ATower_DefenseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATower_DefenseCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATower_DefenseCharacter::Look);

		// Shooting
		EnhancedInputComponent->BindAction(PushAction, ETriggerEvent::Started, this, &ATower_DefenseCharacter::PushEnemy);
		
	}

}

void ATower_DefenseCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ATower_DefenseCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ATower_DefenseCharacter::PushEnemy()
{
	FHitResult OutHit;

	AController* PlayerController = GetController();
	//Determines where the Beam start and end locations are.
	FRotator BeamRotation = FollowCamera->GetRelativeRotation();
	FVector BeamStart = GetActorLocation();
	FVector ForwardVector = FollowCamera->GetForwardVector();
	FVector BeamEnd = ((ForwardVector * 1800.f) + BeamStart);
	FVector PushBackDirection = BeamStart - (BeamStart.Z - 50.f);
	
	//Initialize Collision parameters
	FCollisionQueryParams CollisionParams;

	DrawDebugLine(GetWorld(), BeamStart, BeamEnd, FColor::Red, false, 3.0f, 0, 1);

	//Execute a LineTrace with OutHit as result
	GetWorld()->LineTraceSingleByChannel(OutHit, BeamStart, BeamEnd, TraceChannelProperty, CollisionParams);

	// If the trace hit something, bBlockingHit will be true,
	// and its fields will be filled with detailed info about what was hit
	if (OutHit.bBlockingHit && IsValid(OutHit.GetActor()))
	{
		if(OutHit.GetActor() != nullptr)
		{
			//TODO: This does not push enemies away, it only teleports them up.
			FVector VectorToEnemy = GetActorLocation() + (OutHit.GetActor()->GetActorLocation() - GetActorLocation());
			FVector PushBackEndPoint = VectorToEnemy + (GetActorLocation().ZAxisVector * 200.f);
			OutHit.GetActor()->SetActorLocation(PushBackEndPoint);
			//OutHit.GetComponent()->AddImpulse(ForwardVector*1000,FName("NAME_None"), true);
			/*UPrimitiveComponent* component = Cast<UPrimitiveComponent>(OutHit.GetActor()->GetRootComponent());
			if (component != nullptr)
			{
				component->AddImpulseAtLocation(ForwardVector*500, BeamEnd);
			}
			OutHit.GetActor()->GetRootComponent()->AddLocalRotation(BeamRotation);*/
			//SetActorLocation(GetActorLocation() + (Direction * MoveSpeed * GetWorld()->GetDeltaSeconds()));
			DrawDebugLine(GetWorld(), BeamStart, PushBackEndPoint, FColor::Cyan, false, 2.0f, 0, 1);
		}
	}
}