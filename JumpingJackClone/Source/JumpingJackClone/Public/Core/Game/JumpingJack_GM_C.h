// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "JumpingJack_GM_C.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameWin);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnGameLose);

class AMapController_C;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class JUMPINGJACKCLONE_API AJumpingJack_GM_C : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	int DifficultyLevel;

	UPROPERTY(BlueprintReadOnly, VisibleInstanceOnly, meta = (AllowPrivateAccess = "true"))
	AMapController_C* MapControllerRef;
	
public:
	AJumpingJack_GM_C();
	
	UFUNCTION(BlueprintCallable)
	void RegisterMapController(AMapController_C* MapRef);
	
	virtual void ResetLevel() override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerWin();
	virtual void TriggerWin_Implementation();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void TriggerLose();
	virtual void TriggerLose_Implementation();

	UFUNCTION(BlueprintCallable)
	void SetDifficultyLevel(int NewDifficultyLevel);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddScore(AController* Player, int Value);
	virtual void AddScore_Implementation(AController* Player, int Value);

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnGameWin OnGameWin;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnGameLose OnGameLose;
};
