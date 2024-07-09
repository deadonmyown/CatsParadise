// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "CatsParadiseBaseCharacter.h"

#include "Logging/LogMacros.h"
#include "CatsParadiseCharacter.generated.h"



class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;
class UInteractionQueueComponent;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ACatsParadiseCharacter : public ACatsParadiseBaseCharacter
{
	GENERATED_BODY()

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* MovementMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* BoatMappingContext;

	/** Move Boat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveBoatAction;

	/** Steer Boat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* SteerBoatAction;

	/** Steer Boat Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* LeaveBoatAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;

	/** Interact Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputMappingContext* ItemMappingContext;

	/** Use Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* UseAction;

	/** Drop Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* DropAction;
	
public:
	ACatsParadiseCharacter();

protected:
	virtual void BeginPlay();

public:
	//Interaction Queue Component
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Character", meta = (AllowPrivateAccess = "true"))
	UInteractionQueueComponent* InteractionQueueComponent;

	/** Called for looking input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void Look(const FInputActionValue& Value);

	/** Called for interacting input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void Interact();

protected:

	/** Called for movement input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void CustomJump();

	/** Called for movement input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void CustomStopJumping();
	
	/** Called for movement input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void Move(const FInputActionValue& Value);

	/** Called for movement boat input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void MoveBoat(const FInputActionValue& Value);

	/** Called for movement steer input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void SteerBoat(const FInputActionValue& Value);

	/** Called for interacting input */
	UFUNCTION(BlueprintNativeEvent, Category="Character")
	void LeaveBoat();
	
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
};

