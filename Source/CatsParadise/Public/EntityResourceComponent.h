// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EntityResourceComponent.generated.h"

USTRUCT(BlueprintType)
struct FResourceData
{
	GENERATED_BODY()

	/**
	 * Current value.
	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="EntityResourceComponent")
	int32 Value = 100;

	/**
	 * Max value.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResourceComponent", meta=(ClampMin="0"))
	int32 MaxValue = 100;

	/**
	 * If true Value = InitialValue, else Value = MaxValue on resource creation.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="EntityResourceComponent")
	bool bUseCustomInitialValue = false;

	UPROPERTY(EditAnywhere,
		BlueprintReadWrite,
		Category="EntityResourceComponent",
		meta=(EditCondition="bCustomInitialValue", ClampMin="0"))
	int32 InitialValue = 100;
	
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResourceValueChange, int32, NewValue, int32, Amount);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnResourceValueZero);

UCLASS( Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CATSPARADISE_API UEntityResourceComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEntityResourceComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void InitializeComponent() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, Category="EntityResourceComponent")
	FOnResourceValueChange OnValueDecreased;

	UPROPERTY(BlueprintAssignable, Category="EntityResourceComponent")
	FOnResourceValueChange OnValueIncreased;

	UPROPERTY(BlueprintAssignable, Category="EntityResourceComponent")
	FOnResourceValueZero OnValueZero;

	UPROPERTY(BlueprintAssignable, Category="EntityResourceComponent")
	FOnResourceValueChange OnMaxValueDecreased;

	UPROPERTY(BlueprintAssignable, Category="EntityResourceComponent")
	FOnResourceValueChange OnMaxValueIncreased;

	UFUNCTION(BlueprintCallable, Category="EntityResourceComponent")
	bool DecreaseValue(const int32 Amount);

	/**
	 * Increases value. If ClampToMax == true, it'll be clamped to MaxValue.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResourceComponent")
	bool IncreaseValue(const int32 Amount, bool bClampToMax = true);

	/**
	 * Decreases MaxValue. If ClampValue == true and Value > MaxValue, Value will be clamped to MaxValue.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResourceComponent")
	bool DecreaseMaxValue(int32 Amount, const bool bClampValue = true);

	/**
	 * Increases MaxValue. If ClampValue == true and Value < MaxValue, Value will be clamped to MaxValue.
	 */
	UFUNCTION(BlueprintCallable, Category="EntityResourceComponent")
	bool IncreaseMaxValue(int32 Amount, const bool bClampValue = true);

	UFUNCTION(BlueprintPure, Category="EntityResourceComponent")
	int32 GetValue() const;

	UFUNCTION(BlueprintPure, Category="EntityResourceComponent")
	int32 GetMaxValue() const;

	void SetResourceData(const FResourceData& Data);

	void GetResourceData(FResourceData& Data) const;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="EntityResourceComponent", meta=(AllowPrivateAccess))
	FResourceData ResourceData;
};
