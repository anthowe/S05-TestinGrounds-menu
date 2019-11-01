// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuProjectGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API ALobbyGameMode : public AMenuProjectGameMode
{
	GENERATED_BODY()
	
public:
	ALobbyGameMode();
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(APlayerController* Exiting);

private:
	void StartGame();
	
	uint32 NumberOfPlayers = 0;
	
	FTimerHandle GetStartTimer;
};
