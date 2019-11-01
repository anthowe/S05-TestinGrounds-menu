// Fill out your copyright notice in the Description page of Project Settings.

#include "MenuProjectGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

#include "OnlineSessionSettings.h"

//#include "PlatformTrigger.h"
#include "MenuSystem/MainMenu.h"
#include "MenuSystem/GameMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName NAME_GAME_SESSION = TEXT("GameSession");
const static FName SERVER_NAMES_SETTINGS_KEY = TEXT("ServerName");

UMenuProjectGameInstance::UMenuProjectGameInstance(const FObjectInitializer & ObjectInitializer)
{
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/Menu/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	
	MenuClass = MenuBPClass.Class;
	
	ConstructorHelpers::FClassFinder<UUserWidget> GameMenuBPClass(TEXT("/Game/Menu/WBP_GameMenu"));
	if (!ensure(GameMenuBPClass.Class != nullptr)) return;

	GameMenuClass = GameMenuBPClass.Class;
}

void UMenuProjectGameInstance::Init()
{
	IOnlineSubsystem*Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		
	}
	SessionInterface = Subsystem->GetSessionInterface();
	if (SessionInterface.IsValid())
		
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMenuProjectGameInstance::OnCreateSessionComplete);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMenuProjectGameInstance::OnCreateSessionComplete);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMenuProjectGameInstance::OnFindSessionComplete);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMenuProjectGameInstance::OnJoinSessionComplete);
		
		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}
	
}

void UMenuProjectGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);

}

void UMenuProjectGameInstance::GameLoadMenu()
{
	if (!ensure(GameMenuClass != nullptr)) return;
	UMenuWidget* Menu = CreateWidget<UMenuWidget>(this, GameMenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}
void UMenuProjectGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}
void UMenuProjectGameInstance::CreateSession()
{
	if (SessionInterface.IsValid())
	{
		FOnlineSessionSettings SessionSettings;

		if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAMES_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, NAME_GAME_SESSION, SessionSettings);
		
	}
	
}
void UMenuProjectGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if(!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	World->ServerTravel("/Game/Menu/Lobby?listen");
}

void UMenuProjectGameInstance::Host(FString ServerName)
	{	
		DesiredServerName = ServerName;
		if (SessionInterface.IsValid())
		{
		auto ExistingSession = SessionInterface->GetNamedSession(NAME_GAME_SESSION);
		
		if (ExistingSession != nullptr)
		{
			SessionInterface->DestroySession(NAME_GAME_SESSION);
		}
		else
		{
			CreateSession();
		}
		
		}
	}

void UMenuProjectGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}
bool UMenuProjectGameInstance::UpdateSession(FName SessionName, FOnlineSessionSettings & UpdatedSessionSettings, bool bShouldRefreshOnlineData)
{
	return true;
}
void UMenuProjectGameInstance::OnFindSessionComplete(bool Success)
{
	
	
		if (Success && SessionSearch.IsValid() && Menu != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));
			TArray<FServerData > ServerNames;
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				
				UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
				FServerData Data;
			
				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers-SearchResult.Session.NumOpenPublicConnections;
				Data.HostUsername = SearchResult.Session.OwningUserName;
				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAMES_SETTINGS_KEY, ServerName))
				{
					Data.Name = ServerName;
				}
				else
				{
					Data.Name = "Could not find server name.";
				}
				ServerNames.Add(Data);
			}
			Menu->SetServerList(ServerNames);
		}
}
void UMenuProjectGameInstance::Join(uint32 Index)
{
	if (!SessionInterface.IsValid())return;
	if (!SessionSearch.IsValid())return;
	if (Menu != nullptr)
	{
		Menu->Teardown();
	}
	SessionInterface->JoinSession(0, NAME_GAME_SESSION, SessionSearch->SearchResults[Index]);

	
}
void UMenuProjectGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid())return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UMenuProjectGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(NAME_GAME_SESSION);
	}

}

void UMenuProjectGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/Menu/MainMenu", ETravelType::TRAVEL_Absolute);
}