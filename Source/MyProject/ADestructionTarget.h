// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <functional>
#include "GameFramework/Actor.h"
#include "ADestructionTarget.generated.h"

UCLASS()
class MYPROJECT_API AADestructionTarget : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AADestructionTarget();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void DestroyTarget();
	std::function<void(AADestructionTarget*)> DestructionAction;
};
