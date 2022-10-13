// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "CorePlayerState_C.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateLivesRemaining, int32, LivesRemaining);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnReset);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateMaxLives, int32, MaxLives);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateCurrentScore, int32, CurrentScore);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnUpdateHighScore, int32, HighScore);

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class JUMPINGJACKCLONE_API ACorePlayerState_C : public APlayerState
{
	GENERATED_BODY()

	UPROPERTY()
	int32 LivesRemaining;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, Category = "JumpingJack|Player", meta=(AllowPrivateAccess = "true"))
	int32 MaxLives;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	int32 HighScore;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool NewHighScore;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	bool KeepScoreOnReset;
	
public:
	ACorePlayerState_C();
	
	/*
	* Returns the player's high score.
	* @return HighScore
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	int32 GetHighScore() const;

	/*
	* Sets the player's current score.
	* @param Value			The value to set CurrentScore to.
	* @param ModifyByValue	If true, adds Value to CurrentScore. If false, sets CurrentScore to Value
	*/ 
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	void SetCurrentScore(int32 Value, bool ModifyByValue = false);

	/*
	* Sets the player's high score.
	* @param Value			The value to set HighScore to.
	* @param ModifyByValue	If true, adds Value to HighScore. If false, sets HighScore to Value
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	void SetHighScore(int32 Value, bool ModifyByValue = false);
	
	/*
	* Returns the number of lives remaining for the player.
	* @return LivesRemaining
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	int32 GetLivesRemaining() const;

	/*
	* Sets the number of lives remaining for the player.
	* @param Value			The value to set LivesRemaining to.
	* @param ModifyByValue	If true, adds Value to LivesRemaining. If false, sets LivesRemaining to Value
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	void SetLivesRemaining(int32 Value, bool ModifyByValue = true);

	/*
	* Returns the maximum number of lives the player can have.
	* @return MaxLives
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	int32 GetMaxLives() const;

	/*
	* Sets the maximum number of lives the player can have. Optionally can update the player's current number of lives.
	* @param Value			The value to set MaxLives to.
	* @param UpdateCurrent	If true, also updates LivesRemaining to reflect the change. If false, only sets MaxLives
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	void SetMaxLives(int32 Value, bool UpdateCurrent = true);


	/*
	* Changes if the player's score persists through a Reset().
	* @param NewKeepScoreOnReset	If false, the player's score is reset to 0 when Reset() is called.
	*/
	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Player")
	void SetKeepScoreOnReset(bool NewKeepScoreOnReset);

	/*
	* Any pre-game setup is done here.
	*/
	virtual void PostInitializeComponents() override;

	/*
	* Called when the level is reset.
	*/
	virtual void Reset() override;

	// Event delegates for broadcasting changes.
	UPROPERTY(BlueprintAssignable)
	FOnUpdateLivesRemaining OnUpdateLivesRemaining;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateMaxLives OnUpdateMaxLives;

	UPROPERTY(BlueprintAssignable)
	FOnReset OnReset;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateCurrentScore OnUpdateCurrentScore;

	UPROPERTY(BlueprintAssignable)
	FOnUpdateHighScore OnUpdateHighScore;

};
