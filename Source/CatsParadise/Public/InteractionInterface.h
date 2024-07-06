// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * An interface used to implement the interaction functionality.
 */
class CATSPARADISE_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	/**
	 * This function called when the interaction queue component starts the interaction.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void StartInteraction(AActor* OtherActor);

	/**
	 * This function called to activate the interaction effect returns true if success, else false.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	bool FinishInteraction(AActor* OtherActor);

	/**
	 * This function called when the StopInteraction called from the interaction queue component.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interaction")
	void StopInteraction(AActor* OtherActor);
};
