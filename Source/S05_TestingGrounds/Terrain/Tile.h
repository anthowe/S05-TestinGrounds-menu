// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

USTRUCT()
struct FSpawnPosition
{
	
	GENERATED_USTRUCT_BODY()

	FVector Location;
	float Rotation;
	float Scale;
};

class UActorPool;
UCLASS()
class S05_TESTINGGROUNDS_API ATile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATile();


	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceActors(TSubclassOf<AActor> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500, float MinScale =1, float MaxScale = 1);
	
	
	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void PlaceAIPawns(TSubclassOf<APawn> ToSpawn, int MinSpawn = 1, int MaxSpawn = 1, float Radius = 500, float MinScale =1, float MaxScale=1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MinExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Spawning")
	FVector MaxExtent;

	UPROPERTY(EditDefaultsOnly, Category = "Navigation")
	FVector NavigationBoundsOffset;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, Category = "Pool")
	void SetPool(UActorPool* Pool);

	

private:

	void PositionNavMeshBoundsVolume();
	TArray<FSpawnPosition>  RandomSpawnPositions(int MinSpawn, int MaxSpawn, float Radius, float MinScale, float MaxScale);
	
	
	AActor* NavMeshBoundsVolume;
	bool FindEmptyLocation(FVector &FindLocation, float Radius);

	UFUNCTION(Server, Reliable, WithValidation)
	void PlaceActor(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
	void PlaceActor_Implementation(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
	bool PlaceActor_Validate(TSubclassOf<AActor> ToSpawn, FSpawnPosition SpawnPosition);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void PlaceAIPawn(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);
	void PlaceAIPawn_Implementation(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);
	bool PlaceAIPawn_Validate(TSubclassOf<APawn> ToSpawn, FSpawnPosition SpawnPosition);

	bool CanSpawnAtLocation(FVector Location, float Radius);

	UActorPool* Pool;

};
