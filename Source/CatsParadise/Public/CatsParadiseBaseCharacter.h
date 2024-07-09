// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CatsParadiseBaseCharacter.generated.h"


UENUM(BlueprintType)
enum ECharacterState
{
	Default = 0 UMETA(DisplayName = "Default"),
	Vehicle = 0 UMETA(DisplayName = "Vehicle")
};

UENUM(BlueprintType)
enum ECharacterEnvironment
{
	DefaultEnvironment = 0 UMETA(DisplayName = "Default Environment"),
	WaterEnvironment = 1 UMETA(DisplayName = "Water Environment")
};

class USkeletalMeshComponent;

UCLASS()
class CATSPARADISE_API ACatsParadiseBaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	// Sets default values for this character's properties
	ACatsParadiseBaseCharacter();

protected:
	virtual void BeginPlay();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Character")
	bool bHasItem;

	UFUNCTION(BlueprintCallable, Category="Character")
	bool CanTakeItem();

	UFUNCTION(BlueprintGetter, Category="Character")
	bool GetCanTakeItem();

	UFUNCTION(BlueprintSetter, Category="Character")
	void SetCanTakeItem(bool bNewCanTake);
	
	UPROPERTY(VisibleAnywhere, BlueprintGetter=GetCanTakeItem, BlueprintSetter=SetCanTakeItem, Category="Character")
	bool bCanTakeItem;

	UFUNCTION(BlueprintCallable, Category="Character")
	void SetHasItem(bool bNewHasItem);

	UFUNCTION(BlueprintCallable, Category="Character")
	bool GetHasItem();

	UFUNCTION(BlueprintCallable, Category="Character")
	void SetItem(class APickupActor* Item);

	UFUNCTION(BlueprintCallable, Category="Character")
	void ResetItem();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess))
	APickupActor* CurrentItem = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	TEnumAsByte<ECharacterState> CharacterState = ECharacterState::Default;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Character")
	TEnumAsByte<ECharacterEnvironment> CharacterEnvironment = ECharacterEnvironment::DefaultEnvironment;

	UFUNCTION(BlueprintCallable, Category="Character")
	void ChangeCharacterState(ECharacterState NewCharacterState);

	UFUNCTION(BlueprintCallable, Category="Character")
	void ChangeCharacterEnvironment(ECharacterEnvironment NewCharacterEnvironment);

	UFUNCTION(BlueprintCallable, Category="Character")
	void TryUseItem();

	UFUNCTION(BlueprintCallable, Category="Character")
	void DropItem();

	UFUNCTION(BlueprintCallable, Category="Character")
	AActor* GetCharacterInView();

	UFUNCTION(BlueprintCallable, Category="Character")
	bool GetPlayerViewport(FVector& ViewLocation, FRotator& ViewRotation);

	/** The actor caught by line of sight. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Character", meta=(AllowPrivateAccess))
	AActor* ActorInSight = nullptr;

	UPROPERTY(EditAnywhere, Category="Character")
	bool bShouldCheckCharacterInView = false;

	UFUNCTION(BlueprintNativeEvent, Category="Character")
	USkeletalMeshComponent* GetCharacterMesh();

	

protected:
	/** Line of sight trace channel. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Character",
		meta=(AllowPrivateAccess))
	TEnumAsByte<ETraceTypeQuery> TraceChannel = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	/** The line of sight max distance.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	float SightDistance = 512.f;

	/** The line of sight radius. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	float SightRadius = 8.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	TEnumAsByte<EDrawDebugTrace::Type> DebugTrace = EDrawDebugTrace::Type::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character",
		meta=(AllowPrivateAccess))
	float DebugDrawTime = 0.01f;
};
