#include "UTargetsWavesSpawner.h"


UUTargetsWavesSpawner::UUTargetsWavesSpawner()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UUTargetsWavesSpawner::BeginPlay()
{
	Super::BeginPlay();
	FVector actorPosition = GetOwner()->GetActorLocation();
	World = GetWorld();
	SpawnWave(actorPosition, TotalTargetsSpawnCount);
}


void UUTargetsWavesSpawner::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

#if 1 // spin objects, just for fun
	float time = GetOwner()->GetGameTimeSinceCreation();
	float zRotation = FMath::Sin(time) * 50.0f;

	FRotator rotation(0, 0, zRotation);
	rotation *= DeltaTime;

	for (TargetType type : Targets)
	{
		type.Target->AddActorLocalRotation(rotation, false, 0, ETeleportType::None);
	}
#endif
}

// complex logic for wave spawn
void UUTargetsWavesSpawner::SpawnWave(FVector center, int targetsCount)
{
	if (Target == nullptr || World == nullptr)
		return;

	FRotator rotation;
	float minimalDistance = 100.0f;
	FActorSpawnParameters parameters;
	parameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	for (size_t i = 0; i < targetsCount; i++)
	{
		bool isPrimary = i < PrimaryTargetsCount;
		float range = TargetsSpawnRange;
		float from = FMath::Min(PrimaryTargetsSpawnRadius, TargetsSpawnRange);
		float to = FMath::Max(PrimaryTargetsSpawnRadius, TargetsSpawnRange);
		FVector position;

		// apply specific range parameters if this is a primary target
		if (isPrimary)
		{
			from = FMath::Min(100.0f, PrimaryTargetsSpawnRadius);
			to = FMath::Max(100.0f, PrimaryTargetsSpawnRadius);
		}

		do // compare generated location with exisitng targets an see if its too close to any of them. If it is, try again
		{
			float distance = FMath::RandRange(from, to);
			FVector direction = FMath::VRand();
			direction.Z = 0;
			position = (direction * distance) + center;
		} while (!HasEnoughSpaceToSpawnIn(position));

		// spawn new target and bind destruction event to it
		AADestructionTarget* target = World->SpawnActor<AADestructionTarget>(Target, position, rotation);
		target->DestructionAction = std::bind(&UUTargetsWavesSpawner::OnTargetDestroyed, this, target);

		// this struct separates primary and optional targets
		TargetType type;
		type.IsPrimary = isPrimary;
		type.Target = target;

		Targets.Add(type);
	}
}

// event for destroyed target. Invokes next wave if it was a last primary target
void UUTargetsWavesSpawner::OnTargetDestroyed(AADestructionTarget* target)
{
	int removed = Targets.RemoveAll([=](TargetType& element)
		{
			return element.Target == target;
		});

	if (removed < 1)
		return;

	if (!HasActivePrimaryTargets())
		PrepareNextWave();
}

// check if player has any active targets
bool UUTargetsWavesSpawner::HasActivePrimaryTargets()
{
	return Targets.ContainsByPredicate([=](TargetType& element)
		{
			return element.IsPrimary;
		});
}

// clear array, add modification to radius and count, and spawn new wave
void UUTargetsWavesSpawner::PrepareNextWave()
{
	for (TargetType type : Targets)
	{
		type.Target->DestructionAction = nullptr;
		type.Target->DestroyTarget();
	}

	Targets.Reset();
	TargetsSpawnRange *= NewWaveRadiusFactor;
	TotalTargetsSpawnCount *= NewWaveTargetsCountFactor;
	FVector actorPosition = GetOwner()->GetActorLocation();

	SpawnWave(actorPosition, (int)TotalTargetsSpawnCount);
}

// check if position is too close to existing targets
bool UUTargetsWavesSpawner::HasEnoughSpaceToSpawnIn(FVector position)
{
	return !Targets.ContainsByPredicate([=](TargetType& element)
		{
			FVector direction = element.Target->GetActorLocation() - position;
			return direction.Size() < MinimalDistanceBetweenTargets;
		});
}