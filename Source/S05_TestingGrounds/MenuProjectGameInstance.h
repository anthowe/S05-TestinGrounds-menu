// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "MenuProjectGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API UMenuProjectGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UMenuProjectGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init();

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();
	
	UFUNCTION(BlueprintCallable)
	void GameLoadMenu();

	UFUNCTION(Exec)
	void Host(FString ServerName) override;

	UFUNCTION(Exec)
	void Join(uint32 Index) override;

	void StartSession();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;

	bool UpdateSession(FName SessionName, FOnlineSessionSettings & UpdatedSessionSettings, bool bShouldRefreshOnlineData);
	
private:
	TSubclassOf <class UUserWidget> MenuClass;
	TSubclassOf <class UUserWidget> GameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;
	
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;
	void CreateSession();
};