// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>
#include "ADestructionTarget.h"
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "UTargetsWavesSpawner.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MYPROJECT_API UUTargetsWavesSpawner : public USceneComponent
{
	GENERATED_BODY()

public:
	UUTargetsWavesSpawner();

private:
	struct TargetType
	{
		AADestructionTarget* Target;
		bool IsPrimary;
	};

	UWorld* World;
	TArray<TargetType> Targets;
protected:
	virtual void BeginPlay() override;

	void OnTargetDestroyed(AADestructionTarget* target);
	void SpawnWave(FVector center, int targetsCount);
	bool HasActivePrimaryTargets();
	bool HasEnoughSpaceToSpawnIn(FVector position);
	void PrepareNextWave();
public:
	UPROPERTY(EditAnywhere, Category = "Target")
		TSubclassOf<class AADestructionTarget> Target;

	// max spawn radius from player for optional targets
	UPROPERTY(EditAnywhere, Category = "Target")
		float TargetsSpawnRange;

	// total count of all types of targets
	UPROPERTY(EditAnywhere, Category = "Target")
		float TotalTargetsSpawnCount;

	//multiplier for TargetsSpawnRange. Applies after any new wave. 100% - 1.0
	UPROPERTY(EditAnywhere, Category = "Target")
		float NewWaveRadiusFactor;

	//multiplier for TotalTargetsSpawnCount. Applies after any new wave. 100% - 1.0
	UPROPERTY(EditAnywhere, Category = "Target")
		float NewWaveTargetsCountFactor;

	// minimal distance between each spawned target
	UPROPERTY(EditAnywhere, Category = "Target")
		float MinimalDistanceBetweenTargets;

	// primary targets count. New wave starts after all of these destroyed
	UPROPERTY(EditAnywhere, Category = "Primary targets")
		float PrimaryTargetsCount;

	// spawn radius from player of primary targets
	UPROPERTY(EditAnywhere, Category = "Primary targets")
		float PrimaryTargetsSpawnRadius;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
