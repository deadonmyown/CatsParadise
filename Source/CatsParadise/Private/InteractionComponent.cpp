#include "InteractionComponent.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UPrimitiveComponent::SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	UPrimitiveComponent::SetCollisionObjectType(ECC_WorldDynamic);
	UPrimitiveComponent::SetCollisionResponseToAllChannels(ECR_Ignore);
	UPrimitiveComponent::SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}


void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	OnComponentBeginOverlap.AddDynamic(this, &UInteractionComponent::HandleBeginOverlap);
	OnComponentEndOverlap.AddDynamic(this, &UInteractionComponent::HandleEndOverlap);
}


void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UInteractionComponent::SetInteractionMessage(AActor* Actor, const FString& Message)
{
	InteractionData.InteractionMessage = Message;
	UpdateInteractionMessage(Actor, GetOwner(), Message);
}

bool UInteractionComponent::UpdateInteractionMessage(const AActor* Actor, const AActor* InteractiveActor, const FString& NewMessage)
{
	if (!InteractionQueueHasActor(Actor, InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	InteractionQueueComponent->UpdateInteractionMessage(InteractiveActor, NewMessage);
	return true;
}

bool UInteractionComponent::InteractionQueueHasActor(const AActor* Actor, const AActor* InteractiveActor)
{
	if(!IsValid(Actor))
	{
		return false;
	}

	const UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if(!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->QueueHasActor(InteractiveActor);
}

bool UInteractionComponent::AddToInteractionQueue(const AActor* Actor, AActor* InteractiveActor, const FInteractionData& Data)
{
	if (!IsValid(Actor) || !IsValid(InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->Add(InteractiveActor, Data);
}

bool UInteractionComponent::RemoveFromInteractionQueue(const AActor* Actor, const AActor* InteractiveActor)
{
	if (!InteractionQueueHasActor(Actor, InteractiveActor))
	{
		return false;
	}

	UInteractionQueueComponent* InteractionQueueComponent = Actor->FindComponentByClass<UInteractionQueueComponent>();

	if (!InteractionQueueComponent)
	{
		return false;
	}

	return InteractionQueueComponent->Remove(InteractiveActor);
}

void UInteractionComponent::BeginOverlap(AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	if(AddToInteractionQueue(OtherActor, GetOwner(), InteractionData))
	{
		OnActorAdded.Broadcast(OtherActor);
	}
}

void UInteractionComponent::EndOverlap(AActor* OtherActor)
{
	if(!IsValid(OtherActor))
	{
		return;
	}

	if(RemoveFromInteractionQueue(OtherActor, GetOwner()))
	{
		OnActorRemoved.Broadcast(OtherActor);
	}
}

void UInteractionComponent::SetInteractionData(const FInteractionData& Value)
{
	InteractionData = Value;
}

FInteractionData UInteractionComponent::GetInteractionData() const
{
	return InteractionData;
}

void UInteractionComponent::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	BeginOverlap(OtherActor);
}

void UInteractionComponent::HandleEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	EndOverlap(OtherActor);
}

void UInteractionComponent::SetTriggerDefaultCollision(UShapeComponent* ShapeComponent)
{
	if (!ShapeComponent)
	{
		return;
	}

	ShapeComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	ShapeComponent->SetCollisionObjectType(ECC_WorldDynamic);
	ShapeComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
	ShapeComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	ShapeComponent->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	ShapeComponent->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

