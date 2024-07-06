#include "CatsParadiseBaseCharacter.h"
#include "PickupActor.h"
#include "Components/CapsuleComponent.h"

ACatsParadiseBaseCharacter::ACatsParadiseBaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
}

void ACatsParadiseBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ACatsParadiseBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bShouldCheckCharacterInView)
	{
		ActorInSight = GetCharacterInView();
	}
}

void ACatsParadiseBaseCharacter::ChangeCharacterState(ECharacterState NewCharacterState)
{
	CharacterState = NewCharacterState;
}

void ACatsParadiseBaseCharacter::SetCanTakeItem(bool bNewCanTake)
{
	bCanTakeItem = bNewCanTake;
}

bool ACatsParadiseBaseCharacter::CanTakeItem()
{
	if(CharacterState == Default)
	{
		return !GetHasItem();
	}
	else
	{
		return false;
	}
}

bool ACatsParadiseBaseCharacter::GetCanTakeItem()
{
	return bCanTakeItem;
}

void ACatsParadiseBaseCharacter::SetHasItem(bool bNewHasItem)
{
	bHasItem = bNewHasItem;
}

bool ACatsParadiseBaseCharacter::GetHasItem()
{
	return bHasItem;
}

void ACatsParadiseBaseCharacter::SetItem(APickupActor* Item)
{
	if(!IsValid(Item))
	{
		return;
	}
	
	CurrentItem = Item;
	SetHasItem(true);
}

void ACatsParadiseBaseCharacter::ResetItem()
{
	CurrentItem = nullptr;
	SetHasItem(false);
}

void ACatsParadiseBaseCharacter::TryUseItem()
{
	if(!IsValid(CurrentItem))
	{
		return;
	}

	CurrentItem->TryUseItem();
}

void ACatsParadiseBaseCharacter::DropItem()
{
	if(!IsValid(CurrentItem))
	{
		return;
	}

	CurrentItem->DropItem();
}

AActor* ACatsParadiseBaseCharacter::GetCharacterInView()
{
	FVector ViewLocation{FVector::ZeroVector};
	FRotator ViewRotation{FRotator::ZeroRotator};

	if(!GetPlayerViewport(ViewLocation, ViewRotation))
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

	UKismetSystemLibrary::LineTraceSingle(GetWorld(),
		TraceStart,
		TraceEnd,
		TraceChannel,
		false,
		{this},
		DebugTrace,
		HitResult,
		true,
		FLinearColor::Red,
		FLinearColor::Green,
		DebugDrawTime);
	
	return HitResult.GetActor();
}

bool ACatsParadiseBaseCharacter::GetPlayerViewport(FVector& ViewLocation, FRotator& ViewRotation)
{
	if(IsPlayerControlled())
	{
		const APlayerController* CharacterController = GetController<APlayerController>();

		if(!CharacterController)
		{
			return false;
		}

		CharacterController->GetPlayerViewPoint(ViewLocation, ViewRotation);
	}

	return true;
}

USkeletalMeshComponent* ACatsParadiseBaseCharacter::GetCharacterMesh_Implementation()
{
	return GetComponentByClass<USkeletalMeshComponent>();
}


