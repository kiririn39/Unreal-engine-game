#include "ADestructionTarget.h"


AADestructionTarget::AADestructionTarget()
{
	PrimaryActorTick.bCanEverTick = true;
}


void AADestructionTarget::BeginPlay()
{
	Super::BeginPlay();

}


void AADestructionTarget::DestroyTarget()
{
	// invoke any additional ligic before destruction
	if (DestructionAction != nullptr)
		DestructionAction(this);

	// here should be invoked particles spawn method
	Destroy();
}


void AADestructionTarget::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}