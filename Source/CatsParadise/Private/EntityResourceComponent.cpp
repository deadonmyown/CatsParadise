#include "EntityResourceComponent.h"

UEntityResourceComponent::UEntityResourceComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	
}


void UEntityResourceComponent::BeginPlay()
{
	Super::BeginPlay();

	ResourceData.Value = ResourceData.bUseCustomInitialValue ? ResourceData.InitialValue : ResourceData.MaxValue;
}

void UEntityResourceComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	ResourceData.Value = ResourceData.bUseCustomInitialValue ? ResourceData.InitialValue : ResourceData.MaxValue;
}


void UEntityResourceComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UEntityResourceComponent::DecreaseValue(const int32 Amount)
{
	if(Amount <= 0 || ResourceData.Value <= 0)
	{
		return false;
	}

	ResourceData.Value -= Amount;
	ResourceData.Value = FMath::Max(ResourceData.Value, 0);
	OnValueDecreased.Broadcast(ResourceData.Value, Amount);

	if(ResourceData.Value == 0)
	{
		OnValueZero.Broadcast();
	}

	return true;
}

bool UEntityResourceComponent::IncreaseValue(const int32 Amount, bool bClampToMax)
{
	if(Amount <= 0 || ResourceData.Value >= ResourceData.MaxValue && bClampToMax)
	{
		return false;
	}

	ResourceData.Value += Amount;

	if(bClampToMax)
	{
		ResourceData.Value = FMath::Min(ResourceData.Value, ResourceData.MaxValue);
	}

	OnValueIncreased.Broadcast(ResourceData.Value, Amount);
	return true;
}

bool UEntityResourceComponent::DecreaseMaxValue(int32 Amount, const bool bClampValue)
{
	if(Amount <= 0 || ResourceData.MaxValue <= 0)
	{
		return false;
	}

	ResourceData.MaxValue -= Amount;
	ResourceData.MaxValue = FMath::Max(ResourceData.MaxValue, 0);
	OnMaxValueDecreased.Broadcast(ResourceData.MaxValue, Amount);

	if(bClampValue && ResourceData.Value > ResourceData.MaxValue)
	{
		Amount = ResourceData.Value - ResourceData.MaxValue;
		DecreaseValue(Amount);
	}

	return true;
}

bool UEntityResourceComponent::IncreaseMaxValue(int32 Amount, const bool bClampValue)
{
	if(Amount <= 0)
	{
		return false;
	}

	ResourceData.MaxValue += Amount;
	OnMaxValueIncreased.Broadcast(ResourceData.MaxValue, Amount);

	if(bClampValue && ResourceData.Value < ResourceData.MaxValue)
	{
		Amount = ResourceData.MaxValue - ResourceData.Value;
		IncreaseValue(Amount, false);
	}

	return true;
}

int32 UEntityResourceComponent::GetValue() const
{
	return ResourceData.Value;
}

int32 UEntityResourceComponent::GetMaxValue() const
{
	return ResourceData.MaxValue;
}

void UEntityResourceComponent::SetResourceData(const FResourceData& Data)
{
	ResourceData = Data;
	ResourceData.Value = ResourceData.bUseCustomInitialValue ? ResourceData.InitialValue : ResourceData.MaxValue;
	OnValueIncreased.Broadcast(ResourceData.Value, 0);
	OnMaxValueIncreased.Broadcast(ResourceData.MaxValue, 0);
}

void UEntityResourceComponent::GetResourceData(FResourceData& Data) const
{
	Data = ResourceData;
}





