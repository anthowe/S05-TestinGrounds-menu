// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"
#include "Math/UnrealMathUtility.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "InfiniteTerrainGameMode.h"
#include "ActorPool.h"
#include "Engine/World.h"
#include "NavigationSystem.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "Net/UnrealNetwork.h"




// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MinExtent = FVector(0, -2000, 0);

	MaxExtent = FVector(4000, 2000, 0);

	NavigationBoundsOffset = FVector(2000, 0, 0);

	SetReplicates(true);
}


void ATile::SetPool(UActorPool* InPool)
{
	UE_LOG(LogTemp, Warning, TEXT("[%s] Setting Pool: %s"), *(this->GetName()), *(InPool->GetName()));
	Pool = InPool;

	PositionNavMeshBoundsVolume();
}

void ATile::PositionNavMeshBoundsVolume()
{
	NavMeshBoundsVolume = Pool->Checkout();

	if (NavMeshBoundsVolume == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Not Enough Actors in the Pool."), *GetName());
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("[%s] CheckedOut"), *GetName(), *NavMeshBoundsVolume->GetName());
	NavMeshBoundsVolume->SetActorLocation(GetActorLocation() + NavigationBoundsOffset);
	FNavigationSystem::Build(*GetWorld());

	

}
TArray<FSpawnPosition>  ATile::RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	TArray<FSpawnPosition> SpawnPositions;
	int NumberToSpawn(FMath::RandRange(MinSpawn, MaxSpawn));
	for (size_t i = 0; i < NumberToSpawn; i++)
	{
		
		FSpawnPosition SpawnPosition;
		SpawnPosition.Scale = FMath::RandRange(MinScale, MaxScale);
		bool found = FindEmptyLocation(SpawnPosition.Location, Radius * SpawnPosition.Scale);
		if (found)
		{
			SpawnPosition.Rotation = FMath::RandRange(-180.f, 180.f);

			SpawnPositions.Add(SpawnPosition);
		}

	}
	return SpawnPositions;
}

void ATile::PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{
	

	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, MinScale, MaxScale);
	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceActor(ToSpawn, SpawnPosition);
	}
	


}



void ATile::PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale)
{

	TArray<FSpawnPosition> SpawnPositions = RandomSpawnPositions(MinSpawn, MaxSpawn, Radius, 1, 1);
	for (FSpawnPosition SpawnPosition : SpawnPositions)
	{
		PlaceAIPawn(ToSpawn, SpawnPosition);
	}
}
bool ATile::FindEmptyLocation(FVector &OutLocation, float Radius)
{
	
	FBox Bounds(MinExtent, MaxExtent);
	

	const int MAX_ATTEMPTS = 100;
	for (size_t i = 0; MAX_ATTEMPTS; i++)
	{
		FVector CandidatePoint = FMath::RandPointInBox(FBox(Bounds));;

		if (CanSpawnAtLocation(CandidatePoint, Radius))
		{
			OutLocation = CandidatePoint;
			return true;
		}
	}
	return false;
	
	
}
//void ATile::PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
//{
//	if (ToSpawn)
//	{
//		AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
//		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
//		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
//		Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));
//	}
//	
//
//}

void  ATile::PlaceActor_Implementation(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	AActor* Spawned = GetWorld()->SpawnActor<AActor>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SetActorScale3D(FVector(SpawnPosition.Scale));

}
bool  ATile::PlaceActor_Validate(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition)
{
	return true;
}

//void ATile::PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
//{
//	if (ToSpawn)
//	{
//		APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
//		Spawned->SetActorRelativeLocation(SpawnPosition.Location);
//		Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
//		Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
//		Spawned->SpawnDefaultController();
//		Spawned->Tags.Add("Enemy");
//
//	}
//
//}

void ATile::PlaceAIPawn_Implementation(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	APawn* Spawned = GetWorld()->SpawnActor<APawn>(ToSpawn);
	Spawned->SetActorRelativeLocation(SpawnPosition.Location);
	Spawned->AttachToActor(this, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false));
	Spawned->SetActorRotation(FRotator(0, SpawnPosition.Rotation, 0));
	Spawned->SpawnDefaultController();
	Spawned->Tags.Add("Enemy");

}
bool ATile::PlaceAIPawn_Validate(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition)
{
	return true;
}

// Called when the game starts or when spawned
void ATile::BeginPlay()
{
	Super::BeginPlay();



}

void ATile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	
	Pool->Return(NavMeshBoundsVolume);
}
	
// Called every frame
void ATile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



bool ATile::CanSpawnAtLocation(FVector Location, float Radius)
{
	FHitResult HitResult;
	FVector GlobalLocation = ActorToWorld().TransformPosition(Location);
	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, GlobalLocation, GlobalLocation, FQuat::Identity,
	ECollisionChannel::ECC_GameTraceChannel2, FCollisionShape::MakeSphere(Radius));
	
	return !HasHit;


}

//
//void ATile::GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps) const
//{
//	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
//
//	
//
//
//}