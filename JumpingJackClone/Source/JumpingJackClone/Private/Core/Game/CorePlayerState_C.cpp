// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Game/CorePlayerState_C.h"

ACorePlayerState_C::ACorePlayerState_C()
{
	MaxLives		= 6;
	LivesRemaining	= 0;
	HighScore		= 0;
	KeepScoreOnReset = false;
}

void ACorePlayerState_C::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	Reset();
}

void ACorePlayerState_C::Reset()
{
	int OnReset_HighScore = GetHighScore();
	int OnReset_CurrentScore = 0;
	int OnReset_LivesRemaining = GetMaxLives();
	if (KeepScoreOnReset){
		OnReset_CurrentScore = (int)GetScore();
		OnReset_LivesRemaining = GetLivesRemaining();
	}
	Super::Reset();
	KeepScoreOnReset = true;
	NewHighScore = false;
	SetLivesRemaining(OnReset_LivesRemaining, false);
	if (OnReset_CurrentScore > OnReset_HighScore) {
		OnReset_HighScore = OnReset_CurrentScore;
	}
	SetCurrentScore(OnReset_CurrentScore);
	SetHighScore(OnReset_HighScore);
	OnReset.Broadcast();
}

void ACorePlayerState_C::SetCurrentScore(int32 Value, bool ModifyByValue) {
	int CurrentScore = (int)GetScore();
	if (Value == CurrentScore && !ModifyByValue) return;
	if (ModifyByValue) {
		CurrentScore += Value;
	}
	else {
		CurrentScore = Value;
	}
	SetScore(CurrentScore);
	OnUpdateCurrentScore.Broadcast((int)GetScore());
	if (!NewHighScore && GetScore() >= GetHighScore()) {
		NewHighScore = true;
	}
	if (NewHighScore) {
		SetHighScore((int)GetScore());
	}
}

int32 ACorePlayerState_C::GetHighScore() const { return HighScore; }
void ACorePlayerState_C::SetHighScore(int32 Value, bool ModifyByValue) {
	if (Value == HighScore && !ModifyByValue) return;
	if (ModifyByValue) {
		HighScore += Value;
	}
	else {
		HighScore = Value;
	}
	OnUpdateHighScore.Broadcast(GetHighScore());
}

int32 ACorePlayerState_C::GetLivesRemaining() const { return LivesRemaining; }
void ACorePlayerState_C::SetLivesRemaining(int32 Value, bool ModifyByValue) {
	if (Value == LivesRemaining && !ModifyByValue) return;
	if (ModifyByValue) {
		LivesRemaining += Value;
	}
	else {
		LivesRemaining = Value;
	}
	OnUpdateLivesRemaining.Broadcast(GetLivesRemaining());
}

int32 ACorePlayerState_C::GetMaxLives() const { return MaxLives; }
void ACorePlayerState_C::SetMaxLives(int32 Value, bool UpdateCurrent)
{
	if (Value == MaxLives) return;
	if (Value <= 0) return;
	if (UpdateCurrent) {
		if (Value > MaxLives) {
			SetLivesRemaining(LivesRemaining + (Value - MaxLives));
		}
		else {
			if (Value < LivesRemaining) {
				SetLivesRemaining(Value);
			}
		}
	}
	MaxLives = Value;
	OnUpdateMaxLives.Broadcast(GetMaxLives());
}

void ACorePlayerState_C::SetKeepScoreOnReset(bool NewKeepScoreOnReset) { KeepScoreOnReset = NewKeepScoreOnReset; }
