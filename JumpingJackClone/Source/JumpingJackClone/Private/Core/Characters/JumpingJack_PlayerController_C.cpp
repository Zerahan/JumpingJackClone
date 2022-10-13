// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/JumpingJack_PlayerController_C.h"
#include "Core/Game/JumpingJack_GM_C.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/Character.h"

AJumpingJack_PlayerController_C::AJumpingJack_PlayerController_C()
	: Super()
{
	
}

void AJumpingJack_PlayerController_C::BeginPlay()
{
	Super::BeginPlay();
	AJumpingJack_GM_C* Gamemode = Cast<AJumpingJack_GM_C>(GetWorld()->GetAuthGameMode());
	Gamemode->OnGameWin.AddDynamic(this, &AJumpingJack_PlayerController_C::OnGameWin);
	Gamemode->OnGameLose.AddDynamic(this, &AJumpingJack_PlayerController_C::OnGameLose);
}

void AJumpingJack_PlayerController_C::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	AJumpingJack_GM_C* Gamemode = Cast<AJumpingJack_GM_C>(GetWorld()->GetAuthGameMode());
	Gamemode->OnGameWin.RemoveDynamic(this, &AJumpingJack_PlayerController_C::OnGameWin);
	Gamemode->OnGameLose.RemoveDynamic(this, &AJumpingJack_PlayerController_C::OnGameLose);
	Super::EndPlay(EndPlayReason);
}

void AJumpingJack_PlayerController_C::Reset()
{
	Super::Reset();
	SetInputMode(FInputModeGameOnly());
	SetShowMouseCursor(false);
}

void AJumpingJack_PlayerController_C::OnGameWin_Implementation()
{
}

void AJumpingJack_PlayerController_C::OnGameLose_Implementation()
{
	GetPawn()->Destroy();
	UnPossess();
	SetInputMode(FInputModeUIOnly());
	SetShowMouseCursor(true);
	if (GameLoseWidgetClass) {
		UUserWidget* GameOverWidget = CreateWidget<UUserWidget>(this, GameLoseWidgetClass);
		if (GameOverWidget) {
			GameOverWidget->AddToViewport();
			//GameOverWidget->SetUserFocus(this);
		}
	}
}
