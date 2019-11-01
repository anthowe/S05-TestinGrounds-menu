// Fill out your copyright notice in the Description page of Project Settings.

#include "GameMenu.h"

#include "Components/WidgetSwitcher.h"
#include "Components/Button.h"

bool UGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success)return false;

	if (!ensure(CancelButton != nullptr)) return false;
	CancelButton->OnClicked.AddDynamic(this, &UGameMenu::CancelPressed);

	if (!ensure(QuitButton != nullptr)) return false;
	QuitButton->OnClicked.AddDynamic(this, &UGameMenu::QuitPressed);

	return true;

}

void UGameMenu::CancelPressed()
{
	Teardown();
}

void UGameMenu::QuitPressed()
{
	if (MenuInterface != nullptr)
	{
		Teardown();
		MenuInterface->LoadMainMenu();
	}
}
