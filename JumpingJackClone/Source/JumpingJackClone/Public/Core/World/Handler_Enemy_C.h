// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/World/Handler_Basic_C.h"
#include "Handler_Enemy_C.generated.h"

class APawn;

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class JUMPINGJACKCLONE_API UHandler_Enemy_C : public UHandler_Basic_C
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<APawn>> EnemyTypes;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<APawn*> SpawnedEnemies;

public:
	UHandler_Enemy_C();

	virtual void SetDifficultyLevel_Implementation(int NewDifficultyLevel);
	virtual void InjectMapSize(const int& _Width, const int& _Height, const uint8& _PlatformCount) override;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void Reset_Implementation(bool ForceFullReset = true) override;
	virtual void FullReset_Implementation() override;
	virtual void Update_Implementation() override;
	virtual void Add_Implementation() override;
};
