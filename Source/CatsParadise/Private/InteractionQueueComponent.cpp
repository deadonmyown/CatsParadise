#include "InteractionQueueComponent.h"

#include "InteractionInterface.h"
#include "GameFramework/Character.h"


UInteractionQueueComponent::UInteractionQueueComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetComponentTickInterval(0.05f);
}


void UInteractionQueueComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UInteractionQueueComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if(!IsQueueEmpty())
	{
		ActorInSight = GetActorInSight();
		SortByLineOfSight(ActorInSight);
	}

}

bool UInteractionQueueComponent::Add(AActor* Actor, const FInteractionData& InteractionData)
{
	if(!IsValid(Actor) && QueueHasActor(Actor) && !HasInteractionInterface(Actor))
	{
		return false;
	}
	
	InteractionQueue.Add(FQueueData{Actor, InteractionData});
	SortInteractionQueue();
	OnActorAdded.Broadcast(Actor);
	return true;
}

bool UInteractionQueueComponent::Remove(const AActor* Actor)
{
	if(!IsValid(Actor) || !QueueHasActor(Actor))
	{
		return false;
	}

	auto Predicate = [&](const FQueueData& Data) { return Data.Actor == Actor; };
	const bool bItemsRemoved = InteractionQueue.RemoveAll(Predicate) > 0;

	if(bItemsRemoved)
	{
		SortInteractionQueue();
		OnActorRemoved.Broadcast(Actor);
	}

	return bItemsRemoved;
}

bool UInteractionQueueComponent::StartInteraction()
{
	if(IsQueueEmpty())
	{
		UE_LOG(LogTemp, Display, TEXT("queue is empty"));
		return false;
	}

	FQueueData Data;
	GetFirstQueueData(Data);

	if(!IsValid(Data.Actor) || !HasInteractionInterface(Data.Actor))
	{
		UE_LOG(LogTemp, Display, TEXT("no interface or is not valid"));
		return false;
	}

	if(Data.InteractionData.bRequireLineOfSight && Data.Actor != ActorInSight)
	{
		UE_LOG(LogTemp, Display, TEXT("line of sight"));
		return false;
	}

	//TODO: If add timer -> add timer logic

	OnInteractionStarted.Broadcast(Data.Actor);
	IInteractionInterface::Execute_StartInteraction(Data.Actor, GetOwner());

	return bFinishManually ? true : FinishInteractionByActor(GetFirstActor());
}

bool UInteractionQueueComponent::FinishInteractionByActor(AActor* Actor)
{
	bool bResult = false;

	if(!QueueHasActor(Actor) || !HasInteractionInterface(Actor))
	{
		return bResult;
	}

	FInteractionData InteractionData;
	GetInteractionData(Actor, InteractionData);

	if(InteractionData.bRequireLineOfSight && Actor != ActorInSight)
	{
		return bResult;
	}

	bResult = IInteractionInterface::Execute_FinishInteraction(Actor, GetOwner());
	
	if(bResult)
	{
		OnInteractionFinished.Broadcast(Actor);
		return bResult;
	}
	
	return bResult;
}

bool UInteractionQueueComponent::FinishInteractionByQueueData(FQueueData& Data)
{
	bool bResult = false;

	if(!QueueHasActor(Data.Actor) || !HasInteractionInterface(Data.Actor))
	{
		return bResult;
	}

	if(Data.InteractionData.bRequireLineOfSight && Data.Actor != ActorInSight)
	{
		return bResult;
	}
	
	bResult = IInteractionInterface::Execute_FinishInteraction(Data.Actor, GetOwner());
	
	if(bResult)
	{
		OnInteractionFinished.Broadcast(Data.Actor);
		return bResult;
	}
	
	return bResult;
}

bool UInteractionQueueComponent::StopInteraction()
{
	FInteractionData InteractionData;
	GetFirstInteractionData(InteractionData);
	AActor* Actor = GetFirstActor();

	if(!IsValid(Actor) || !HasInteractionInterface(Actor))
	{
		return false;
	}

	OnInteractionStopped.Broadcast(Actor);
	IInteractionInterface::Execute_StopInteraction(Actor, GetOwner());
	return true;
}

bool UInteractionQueueComponent::HasInteractionInterface(AActor* Actor)
{
	if(!IsValid(Actor))
	{
		return false;
	}

	return Actor->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass());
}

bool UInteractionQueueComponent::QueueHasActor(const AActor* Actor) const
{
	if(!IsValid(Actor))
	{
		return false;
	}

	auto Predicate = [&](const FQueueData& Data) { return Data.Actor == Actor; };
	return InteractionQueue.ContainsByPredicate(Predicate);
}

bool UInteractionQueueComponent::GetFirstQueueData(FQueueData& QueueData)
{
	if(IsQueueEmpty())
	{
		return false;
	}

	QueueData = InteractionQueue[0];
	return true;
}

bool UInteractionQueueComponent::GetFirstInteractionData(FInteractionData& InteractionData)
{
	if(IsQueueEmpty())
	{
		return false;
	}

	InteractionData = InteractionQueue[0].InteractionData;
	return true;
}

bool UInteractionQueueComponent::UpdateInteractionMessage(const AActor* Actor, const FString& NewMessage)
{
	if(!QueueHasActor(Actor))
	{
		return false;
	}

	FindInteractionData(Actor).InteractionMessage = NewMessage;
	return true;
}

AActor* UInteractionQueueComponent::GetFirstActor()
{
	if(IsQueueEmpty())
	{
		return nullptr;
	}

	return InteractionQueue[0].Actor;
}

bool UInteractionQueueComponent::GetInteractionData(AActor* Actor, FInteractionData& InteractionData)
{
	if(!QueueHasActor(Actor))
	{
		return false;
	}

	InteractionData = FindInteractionData(Actor);
	return true;
}

FInteractionData& UInteractionQueueComponent::FindInteractionData(const AActor* Actor)
{
	auto Predicate = [&](const FQueueData& Data) { return Data.Actor == Actor; };
	return InteractionQueue.FindByPredicate(Predicate)->InteractionData;
}

void UInteractionQueueComponent::SortInteractionQueue()
{
	if(InteractionQueue.Num() <= 1)
	{
		return;
	}

	auto Predicate = [&] (const FQueueData& DataA, const FQueueData& DataB)
	{
		return DataA.InteractionData.bRequireLineOfSight <= DataB.InteractionData.bRequireLineOfSight;
	};
	InteractionQueue.Sort(Predicate);
}

bool UInteractionQueueComponent::IsQueueEmpty()
{
	return InteractionQueue.IsEmpty();
}

bool UInteractionQueueComponent::GetUseLineOfSight() const
{
	return bUseLineOfSight;
}

void UInteractionQueueComponent::SetUseLineOfSight(const bool Value)
{
	bUseLineOfSight = Value;
	SetComponentTickEnabled(bUseLineOfSight);
	SortInteractionQueue();
}

bool UInteractionQueueComponent::GetPlayerViewport(const AActor* Actor, FVector& ViewLocation, FRotator& ViewRotation)
{
	const ACharacter* Character = Cast<ACharacter>(Actor);

	if(!Character)
	{
		return false;
	}

	if(Character->IsPlayerControlled())
	{
		const APlayerController* Controller = Character->GetController<APlayerController>();

		if(!Controller)
		{
			return false;
		}

		Controller->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
}


AActor* UInteractionQueueComponent::GetActorInSight()
{
	FVector ViewLocation{FVector::ZeroVector};
	FRotator ViewRotation{FRotator::ZeroRotator};

	if(!GetPlayerViewport(GetOwner(), ViewLocation, ViewRotation))
	{
		UE_LOG(LogTemp, Display, TEXT("No Player Viewport"));
		return nullptr;
	}

	const FVector TraceStart {ViewLocation};
	const FVector TraceDirection {ViewRotation.Vector()};
	const FVector TraceEnd {TraceStart + TraceDirection * SightDistance};

	if(!GetWorld())
	{
		UE_LOG(LogTemp, Display, TEXT("No World"));
		return nullptr;
	}

	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(),
		TraceStart,
		TraceEnd,
		SightRadius,
		TraceChannel,
		false,
		{GetOwner()},
		DebugTrace,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugDrawTime);
	return HitResult.GetActor();
}

void UInteractionQueueComponent::SortByLineOfSight(const AActor* Actor)
{
	FInteractionData InteractionData;

	if(!IsValid(Actor) || !QueueHasActor(Actor))
	{
		UE_LOG(LogTemp, Display, TEXT("Not Valid Actor"));
		GetFirstInteractionData(InteractionData);

		if(InteractionData.bRequireLineOfSight)
		{
			SortInteractionQueue();
		}
		
		return;
	}

	auto Predicate = [&](const FQueueData& Data) {return Data.Actor == Actor;};
	const FQueueData QueueData = *InteractionQueue.FindByPredicate(Predicate);
	InteractionData = QueueData.InteractionData;

	if(InteractionData.bRequireLineOfSight)
	{
		if(ActorInSight != QueueData.Actor)
		{
			UE_LOG(LogTemp, Display, TEXT("Actor in sight != actor from queueData, stop interaction"));
			StopInteraction();
		}

		const int32 Index = InteractionQueue.IndexOfByPredicate(Predicate);
		InteractionQueue.Swap(Index, 0);
	}
}


