#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tower_DefenseCharacter.generated.h"

class AEnemy;
class ATower;
class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

UCLASS(config = Game)
class ATower_DefenseCharacter : public ACharacter
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* ViewCamera;

    /** Camera boom positioning the camera behind the character */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    USpringArmComponent* CameraBoom;

    /** Follow camera */
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
    UCameraComponent* FollowCamera;

    /** MappingContext */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputMappingContext* DefaultMappingContext;

    /** Jump Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* JumpAction;

    /** Move Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* MoveAction;

    /** Look Input Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* LookAction;

    /** Fire weapon Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* PushAction;

    /** Fire weapon Function */
    UFUNCTION(BlueprintCallable, Category = "Weapon function")
    void PushEnemy();

public:
    ATower_DefenseCharacter();

    /** Preview tower */
    ATower* PreviewTower;

    virtual void Tick(float DeltaSeconds) override;

    /** Tower Placement Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* IA_PlaceTowerAction;

    /** Undo Tower Placement Action */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
    UInputAction* IA_UndoTowerAction;

    /** Tower Blueprint to spawn */
    UPROPERTY(EditAnywhere, Category = "Tower")
    TSubclassOf<ATower> TowerBlueprint;

    
protected:
    /** Called for movement input */
    void Move(const FInputActionValue& Value);

    /** Called for looking input */
    void Look(const FInputActionValue& Value);

    /** Called for placing a tower */
    void PlaceTower();

    /** Called for finalizing tower placement */
    void FinalizeTowerPlacement();

	/** Called for undoing the last tower placement */
    void UndoTowerPlacement();

    //called for moving forward
    void MoveForward(float Value);

    //called for moving to the right
    void MoveRight(float Value);

    //called for turning
    void Turn(float Value);

    //called for looking up
    void LookUp(float Value);

    /** Stack to store placed towers */
    TArray<ATower*> PlacedTowers;

    /** Indicates if the player is in tower placement mode */
    bool bIsPlacingTower;

    /** Timer to manage tower placement cooldown */
    float TowerPlacementCooldownTime = 0.2f; // Cooldown in seconds

    /** Flag to check if placement is on cooldown */
    bool bIsPlacementOnCooldown;

    /** Reset cooldown for tower placement */
    void ResetPlacementCooldown();

    /** Handle to manage cooldown timer */
    FTimerHandle TowerPlacementCooldownHandle;

protected:
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    virtual void BeginPlay();
};
