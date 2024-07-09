#include "CatsParadiseCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractionQueueComponent.h"
#include "Engine/LocalPlayer.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ACatsParadiseCharacter::ACatsParadiseCharacter()
{
	bCanTakeItem = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	//Create Interaction Queue Component
	InteractionQueueComponent=CreateDefaultSubobject<UInteractionQueueComponent>(TEXT("InteractionQueueComponent"));

}

void ACatsParadiseCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(MovementMappingContext, 0);
			Subsystem->AddMappingContext(DefaultMappingContext, 1);
			Subsystem->AddMappingContext(ItemMappingContext, 2);
		}
	}

}

void ACatsParadiseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACatsParadiseCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACatsParadiseCharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::Look);

		//Interacting
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::Interact);
		
		EnhancedInputComponent->BindAction(UseAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::TryUseItem);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::DropItem);

		//Vehicle
		EnhancedInputComponent->BindAction(MoveBoatAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::MoveBoat);
		EnhancedInputComponent->BindAction(SteerBoatAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::SteerBoat);
		EnhancedInputComponent->BindAction(LeaveBoatAction, ETriggerEvent::Triggered, this, &ACatsParadiseCharacter::LeaveBoat);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ACatsParadiseCharacter::Look_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ACatsParadiseCharacter::Interact_Implementation()
{
	if(!Controller || !InteractionQueueComponent)
	{
		return;
	}

	InteractionQueueComponent->StartInteraction();
}

void ACatsParadiseCharacter::CustomJump_Implementation()
{
	Super::Jump();
}

void ACatsParadiseCharacter::CustomStopJumping_Implementation()
{
	Super::StopJumping();
}

void ACatsParadiseCharacter::Move_Implementation(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ACatsParadiseCharacter::MoveBoat_Implementation(const FInputActionValue& Value)
{
}

void ACatsParadiseCharacter::SteerBoat_Implementation(const FInputActionValue& Value)
{
}

void ACatsParadiseCharacter::LeaveBoat_Implementation()
{
}
