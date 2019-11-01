// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "Public/TimerManager.h"

#include "Engine/World.h"
#include "MenuProjectGameInstance.h"
#include "UObject/ConstructorHelpers.h"

ALobbyGameMode::ALobbyGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Dynamic/Character/Behavior/BP_Character"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

}


void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;
	if (NumberOfPlayers >= 2)
	{
		GetWorldTimerManager().SetTimer(GetStartTimer, this, &ALobbyGameMode::StartGame, 20);
		
	}
}

void ALobbyGameMode::Logout(APlayerController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast <UMenuProjectGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;

	World->ServerTravel("/Game/Menu/StaticMap?listen");
}