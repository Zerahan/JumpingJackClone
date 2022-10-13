// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TeleportInterface.generated.h"

class UPrimitiveComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI, Blueprintable)
class UTeleportInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class JUMPINGJACKCLONE_API ITeleportInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "JumpingJack|Teleport")
	int GetTeleportOffset() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "JumpingJack|Teleport")
	bool IsTeleportCollider(UPrimitiveComponent* ComponentToCheck) const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "JumpingJack|Teleport")
	void TeleportTo(FTransform TargetTransform);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "JumpingJack|Teleport")
	bool GetIsTeleporting() const;

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "JumpingJack|Teleport")
	void SetIsTeleporting(bool NewIsTeleporting);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "JumpingJack|Teleport")
	float GetEntityHalfWidth() const;
};
