// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Game/JumpingJack_GM_C.h"
#include "Core/Game/CorePlayerState_C.h"
#include "Core/Characters/CharacterBasic_C.h"
#include "Core/World/MapController_C.h"
#include "Core/Characters/JumpingJack_PlayerController_C.h"

AJumpingJack_GM_C::AJumpingJack_GM_C()
{
	DefaultPawnClass = ACharacterBasic_C::StaticClass();
	PlayerStateClass = ACorePlayerState_C::StaticClass();
	PlayerControllerClass = AJumpingJack_PlayerController_C::StaticClass();
	DifficultyLevel = 0;
}

void AJumpingJack_GM_C::RegisterMapController(AMapController_C* MapRef) {
	MapControllerRef = MapRef;
}

void AJumpingJack_GM_C::ResetLevel() {
	Super::ResetLevel();
	for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		APlayerController* PlayerCon = Cast<APlayerController>(Iterator->Get());
		if (IsValid(PlayerCon)) {
			RestartPlayer(PlayerCon);
			MapControllerRef->Reset();
		}
	}
}

void AJumpingJack_GM_C::TriggerWin_Implementation()
{
	ResetLevel();
	SetDifficultyLevel(DifficultyLevel + 1);
	OnGameWin.Broadcast();
}

void AJumpingJack_GM_C::TriggerLose_Implementation()
{
	SetDifficultyLevel(0);
	OnGameLose.Broadcast();
	for (auto Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator) {
		APlayerController* PlayerCon = Cast<APlayerController>(Iterator->Get());
		if (IsValid(PlayerCon)) {
			ACorePlayerState_C* PlayerState = PlayerCon->GetPlayerState<ACorePlayerState_C>();
			if(PlayerState){
				PlayerState->SetKeepScoreOnReset(false);
			}
		}
	}
}

inline void AJumpingJack_GM_C::SetDifficultyLevel(int NewDifficultyLevel) {
	if (DifficultyLevel == NewDifficultyLevel) return;
	DifficultyLevel = NewDifficultyLevel;
	if (IsValid(MapControllerRef)) {
		MapControllerRef->SetDifficultyLevel(DifficultyLevel);
	}
}

void AJumpingJack_GM_C::AddScore_Implementation(AController* Player, int Value)
{
	ACorePlayerState_C* PlayerState = Player->GetPlayerState<ACorePlayerState_C>();
	if (PlayerState) {
		PlayerState->SetCurrentScore(Value * (DifficultyLevel + 1), true);
	}
}
