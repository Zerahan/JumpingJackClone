// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "JumpingJack_PlayerController_C.generated.h"

class UUserWidget;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class JUMPINGJACKCLONE_API AJumpingJack_PlayerController_C : public APlayerController
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameLoseWidgetClass;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> GameWinWidgetClass;

public:
	AJumpingJack_PlayerController_C();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	virtual void Reset() override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameWin();
	virtual void OnGameWin_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnGameLose();
	virtual void OnGameLose_Implementation();
};
