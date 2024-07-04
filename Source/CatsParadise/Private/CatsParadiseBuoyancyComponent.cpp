// Fill out your copyright notice in the Description page of Project Settings.

#include "CatsParadiseBuoyancyComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
UCatsParadiseBuoyancyComponent::UCatsParadiseBuoyancyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	ParentActor = GetOwner();
}


// Called when the game starts
void UCatsParadiseBuoyancyComponent::BeginPlay()
{
	Super::BeginPlay();
	ActorTransform = ParentActor->GetActorTransform();
	WorldActorLocation = ParentActor->GetActorLocation();
	WorldActorRotation = ParentActor->GetActorRotation();
	if (MyStaticMeshComponent->IsValidLowLevelFast())
	{
		RelativeStaticMeshLocation = MyStaticMeshComponent->GetRelativeLocation();
		RelativeStaticMeshRotation = MyStaticMeshComponent->GetRelativeRotation();
	}
	ParentActor->SetActorLocation(FVector(WorldActorLocation.X, WorldActorLocation.Y, 0));
	FFTCalculator = InitializeWaterZoneReference();
}

// Called every frame
void UCatsParadiseBuoyancyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bWaterZoneValid)
	{
		if (ActorTransform.GetLocation() != ParentActor->GetActorLocation())
		{
			ActorTransform = ParentActor->GetActorTransform();
			WorldActorLocation = ParentActor->GetActorLocation();
			WorldActorRotation = ParentActor->GetActorRotation();
			ParentActor->SetActorLocation(FVector(WorldActorLocation.X, WorldActorLocation.Y, 0));
		}

		if (PontoonsLocations.Num() > 2)
		{
			const TArray<FVector> BuoyancyArray = GetBuoyancyArray(PontoonsLocations);
			const FQuat ActorQuat = CalculateBuoyancyRotation(BuoyancyArray);
			const FVector BuoyancyLocation = GetMultiBuoyancyLocation(PontoonsLocations);
			const FRotator BuoyancyRotation = ActorQuat.Rotator() * RotationStrength + WorldActorRotation;

			if (MyStaticMeshComponent->IsValidLowLevelFast())
			{
				MyStaticMeshComponent->SetWorldLocation(BuoyancyLocation);
				MyStaticMeshComponent->SetWorldRotation(BuoyancyRotation);
			}

			if (DebugPoints) { DrawBuoyancyArrayDebugPoints(BuoyancyArray); }
		}
		else
		{
			if (MyStaticMeshComponent->IsValidLowLevelFast())
			{
				const FVector BuoyancyLocation = GetBuoyancyLocation(PontoonsLocations[0]);
				MyStaticMeshComponent->SetWorldLocation(BuoyancyLocation);
			}
		}
	}
}



FVector UCatsParadiseBuoyancyComponent::GetBuoyancyLocation(FVector RelativeLocation)
{
	FVector BuoyancyLocation = FVector::ZeroVector;
	FVector WorldLocation = ActorTransform.TransformPosition(RelativeLocation);
	if (FFTCalculator == nullptr) { return BuoyancyLocation; }
	else {

		//FVector GridPointLocation = FVector(WorldLocation.X, WorldLocation.Y, -RelativeLocation.Z) / FFTCalculator->Scale * FFTCalculator->MultiplyScale;
		FVector GridPointLocation = FVector(WorldLocation.X, WorldLocation.Y, -RelativeLocation.Z) ;
		FVector Displacement = FFTCalculator->GetDisplacementAtPoint(GridPointLocation);
		
		//BuoyancyLocation = GridPointLocation * FFTCalculator->Scale / FFTCalculator->MultiplyScale + Displacement / FFTCalculator->Scale / FFTCalculator->OverlapScale;
		BuoyancyLocation = GridPointLocation + Displacement;
		
		return BuoyancyLocation;
	}
}

FVector UCatsParadiseBuoyancyComponent::GetMultiBuoyancyLocation(TArray<FVector> PontoonsArray)
{
	FVector BuoyancyLocation = FVector::ZeroVector;
	FVector AveragePontoons = FindAverageLocation(PontoonsArray);

	BuoyancyLocation = GetBuoyancyLocation(AveragePontoons);
	return BuoyancyLocation;
}



FOceanFFTCalculator* UCatsParadiseBuoyancyComponent::InitializeWaterZoneReference()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOceanWaterZone::StaticClass(), FoundActors);

	if (FoundActors.Num() > 0)
	{
		for (AActor* FoundActor : FoundActors)
		{
			OceanWaterZone = Cast<AOceanWaterZone>(FoundActor);
			if (OceanWaterZone)
			{
				return &OceanWaterZone->FFTCalculator;
			}
		}
	}
	bWaterZoneValid = false;
	UE_LOG(LogTemp, Warning, TEXT("WaterZone isn't valid"));
	return nullptr;
}

FVector UCatsParadiseBuoyancyComponent::FindAverageLocation(TArray<FVector> Locations)
{
	FVector AverageLocation = FVector::ZeroVector;
	for (FVector& Location : Locations)
	{
		AverageLocation = AverageLocation + Location;
	}
	AverageLocation = AverageLocation / Locations.Num();
	return AverageLocation;
}

TArray<FVector> UCatsParadiseBuoyancyComponent::GetBuoyancyArray(TArray<FVector> Points)
{
	TArray<FVector> PointArray = {};
	for (FVector& Point : Points)
	{
		PointArray.Add(GetBuoyancyLocation(Point));
	}
	return PointArray;
}

FQuat UCatsParadiseBuoyancyComponent::CalculateBuoyancyRotation(const TArray<FVector> Points)
{
	FQuat AverageRotation = FQuat::Identity;

	for (const FVector& WavePoint : Points)
	{
		float DistanceToCenter = FVector::Dist(ParentActor->GetActorLocation(), WavePoint);
		float Weight = 1.0f / (DistanceToCenter + SMALL_NUMBER); 

		FQuat WaveRotation = CalculateWaveRotation(WavePoint);

		// Accumulate the weighted rotation
		AverageRotation = FQuat::Slerp(AverageRotation, WaveRotation, Weight);
	}
	return AverageRotation;
}

FQuat UCatsParadiseBuoyancyComponent::CalculateWaveRotation(const FVector& WavePoint)
{
	FVector TargetVector(0.0f, 0.0f, 1.0f);
	FVector WaveDirection = (WavePoint - ParentActor->GetActorLocation()).GetSafeNormal();
	FQuat WaveRotation = FQuat::FindBetween(WaveDirection, TargetVector);

	return WaveRotation;
}

void UCatsParadiseBuoyancyComponent::DrawBuoyancyArrayDebugPoints(const TArray<FVector>& BuoyancyArray)
{
	for (const FVector& Point : BuoyancyArray)
	{
		DrawDebugPoint(GetWorld(), Point, 50.f, FColor(255.f, 0.f, 0.f, 255.f), false, 0.f, 0);
	}
}