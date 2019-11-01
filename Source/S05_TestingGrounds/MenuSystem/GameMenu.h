// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"


#include "MenuWidget.h"
#include "GameMenu.generated.h"

/**
 * 
 */
UCLASS()
class S05_TESTINGGROUNDS_API UGameMenu : public UMenuWidget
{
	GENERATED_BODY()

protected:

	virtual bool Initialize();
private:

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	UFUNCTION()
	void CancelPressed();

	UFUNCTION()
	void QuitPressed();
	
};
