// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "InteractionQueueComponent.generated.h"

USTRUCT(BlueprintType)
struct FInteractionData
{
	GENERATED_BODY()

	/**
	 * Toggles if the actor require being in the line of sight to be interacted. 
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	bool bRequireLineOfSight = false;

	/**
	 * A message which can be used in HUD.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData")
	FString InteractionMessage = "Interact";

	/**
	 * How much time required to activate interaction effect.
	 */
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="InteractionData", meta=(ClampMin="0"))
	float InteractionTime = 0.f;*/
};

USTRUCT(BlueprintType)
struct FQueueData
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QueueData")
	AActor* Actor = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QueueData")
	FInteractionData InteractionData;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, AActor*, TargetActor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQueueChanged, const AActor*, OtherActor);

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSPARADISE_API UInteractionQueueComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionQueueComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	/** Called when the interaction process started. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteraction OnInteractionStarted;

	/** Called when the interaction effect successfully activated. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteraction OnInteractionFinished;

	/** Called when the interaction process stopped. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnInteraction OnInteractionStopped;

	/** Called when actor was added to the queue. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnQueueChanged OnActorAdded;

	/** Called when actor was added to the queue. */
	UPROPERTY(BlueprintAssignable, Category="Interaction")
	FOnQueueChanged OnActorRemoved;

	/** Adds interaction data into the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool Add(AActor* Actor, const FInteractionData& InteractionData);

	/** Removes interaction data into the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool Remove(const AActor* Actor);

	/** Starts interaction with the first actor in the interaction queue. */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool StartInteraction();

	/** Finishes interaction.
	 * Call it if FinishManually == false.
	 */
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool FinishInteractionByActor(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool FinishInteractionByQueueData(FQueueData& Data);

	/** Stops interaction.*/
	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool StopInteraction();

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool HasInteractionInterface(AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool QueueHasActor(const AActor* Actor) const;

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool GetFirstQueueData(FQueueData& QueueData);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool GetFirstInteractionData(FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	AActor* GetFirstActor();

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool GetInteractionData(AActor* Actor, FInteractionData& InteractionData);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	FInteractionData& FindInteractionData(const AActor* Actor);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	void SortInteractionQueue();

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool IsQueueEmpty();

	UFUNCTION(BlueprintGetter, Category="InteractionSystem")
	bool GetUseLineOfSight() const;

	UFUNCTION(BlueprintSetter, Category="InteractionSystem")
	void SetUseLineOfSight(const bool Value);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool GetPlayerViewport(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation);

private:
	/** If true, the FinishInteraction() must be called manually.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	bool bFinishManually = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	TArray<FQueueData> InteractionQueue;

	/**
	 * Toggles the line of sight checks.
	 * Keep it false if there's no interactive actors require line of sight to interact with.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintGetter=GetUseLineOfSight, BlueprintSetter=SetUseLineOfSight,
		Category="Interaction", meta=(AllowPrivateAccess))
	bool bUseLineOfSight = false;

	/** Line of sight trace channel. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	/** The line of sight max distance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightDistance = 512.f;

	/** The line of sight radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float SightRadius = 32.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Interaction",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	TEnumAsByte<EDrawDebugTrace::Type> DebugTrace = EDrawDebugTrace::Type::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess, EditCondition="bUseLineOfSight"))
	float DebugDrawTime = 0.05f;

	/** The actor caught by line of sight. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Interaction", meta=(AllowPrivateAccess))
	AActor* ActorInSight = nullptr;

	AActor* GetActorInSight();

	void SortByLineOfSight(const AActor* Actor);
		
};
