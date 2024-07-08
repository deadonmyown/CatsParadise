#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OceanManager.generated.h"

/**
 *
 */
UCLASS()
class CATSPARADISE_API AOceanManager : public AActor
{
 GENERATED_BODY()

public:

 AOceanManager();

 UPROPERTY(BlueprintReadWrite, Category = "General", EditAnywhere)
 FVector Center;

 UPROPERTY(BlueprintReadWrite, Category = "General", EditAnywhere)
  float Size;

 UPROPERTY(BlueprintReadWrite, Category = "General", EditAnywhere)
  float MaxHeight;

 // Ocean
 UPROPERTY(BlueprintReadWrite, Category = "Ocean", EditAnywhere)
  UTexture2D* OceanDepthTexture;

 UPROPERTY(BlueprintReadWrite, Category = "Ocean", EditAnywhere)
  float Wavelength;

 UPROPERTY(BlueprintReadWrite, Category = "Ocean", EditAnywhere)
  float Amplitude;

 UPROPERTY(BlueprintReadWrite, Category = "Ocean", EditAnywhere)
  FVector2D Direction;

 UFUNCTION(BlueprintCallable, Category = "Ocean")
  void Initialize();

 UFUNCTION(BlueprintCallable, Category = "Ocean")
  FVector GetWaveHeightValue(FVector location, float time);

private:
 
 FVector CalculateGerstnerWave(float wavelength, float amplitude, FVector2D position, FVector2D direction, float angle, float steepness, float time, float phase);
 FVector CalculateGerstnerWaveCluster(float medianWavelength, float medianAmplitude, FVector2D position, FVector2D medianDirection, float medianAngle, float steepness, float time, float medianPhase);

};