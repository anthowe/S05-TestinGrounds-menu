// Fill out your copyright notice in the Description page of Project Settings.

#include "ActorPool.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "S05_TestingGrounds.h"

// Sets default values for this component's properties
UActorPool::UActorPool()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
	
}


AActor* UActorPool::Checkout()
{
	
	if (Pool.Num() == 0)
	{
		return nullptr;
	}
	return Pool.Pop();
}

void UActorPool::Return(AActor* ActorToReturn)
{
	Add(ActorToReturn);
}

void UActorPool::Add(AActor* ActorToAdd)
{
	if (ActorToAdd)
	{
		Pool.Push(ActorToAdd);
	}

	
}

void UActorPool::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UActorPool, Pool);
}
