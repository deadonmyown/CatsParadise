// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractionQueueComponent.h"
#include "Components/SphereComponent.h"
#include "InteractionComponent.generated.h"


UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSPARADISE_API UInteractionComponent : public USphereComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInteractionComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category="InteractionSystem")
	FOnQueueChanged OnActorAdded;

	UPROPERTY(BlueprintAssignable, Category="InteractionSystem")
	FOnQueueChanged OnActorRemoved;

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	void SetInteractionMessage(AActor* Actor, const FString& Message);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool UpdateInteractionMessage(const AActor* Actor, const AActor* InteractiveActor, const FString& NewMessage);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool InteractionQueueHasActor(const AActor* Actor, const AActor* InteractiveActor);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool AddToInteractionQueue(const AActor* Actor, AActor* InteractiveActor, const FInteractionData& Data);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	bool RemoveFromInteractionQueue(const AActor* Actor, const AActor* InteractiveActor);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	void BeginOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	void EndOverlap(AActor* OtherActor);

	UFUNCTION(BlueprintSetter, Category="InteractionSystem")
	void SetInteractionData(const FInteractionData& Value);

	UFUNCTION(BlueprintGetter, Category="InteractionSystem")
	FInteractionData GetInteractionData() const;

	UFUNCTION(BlueprintCallable, Category="InteractionSystem")
	static void SetTriggerDefaultCollision(UShapeComponent* ShapeComponent);
	
private:
	UPROPERTY(EditDefaultsOnly,
		BlueprintGetter=GetInteractionData,
		BlueprintSetter=SetInteractionData,
		Category="Interaction",
		meta=(AllowPrivateAccess))
	FInteractionData InteractionData;

	UFUNCTION()
	virtual void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
									AActor* OtherActor,
									UPrimitiveComponent* OtherComp,
									int32 OtherBodyIndex,
									bool bFromSweep,
									const FHitResult& SweepResult);

	UFUNCTION()
	virtual void HandleEndOverlap(UPrimitiveComponent* OverlappedComponent,
								  AActor* OtherActor,
								  UPrimitiveComponent* OtherComp,
								  int32 OtherBodyIndex);
};
