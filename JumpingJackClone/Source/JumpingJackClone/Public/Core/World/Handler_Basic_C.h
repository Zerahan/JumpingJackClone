// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "Handler_Basic_C.generated.h"


UCLASS( BlueprintType, Blueprintable, Abstract)
class JUMPINGJACKCLONE_API UHandler_Basic_C : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHandler_Basic_C();

protected:
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta=(BlueprintProtected))
	bool DoFullReset;

	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (BlueprintProtected))
	int MapWidth;
	
	UPROPERTY(BlueprintReadWrite, EditInstanceOnly, meta = (BlueprintProtected))
	int MapHeight;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	uint8 PlatformCount;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float SpaceBetweenPlatforms;
	
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDifficultyLevel(int NewDifficultyLevel);
	virtual void SetDifficultyLevel_Implementation(int NewDifficultyLevel);

	UFUNCTION(BlueprintCallable)
	virtual void InjectMapSize(const int& _Width, const int& _Height, const uint8& _PlatformCount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void Reset(bool ForceFullReset = true);
	virtual void Reset_Implementation(bool ForceFullReset = true);

	UFUNCTION(BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void FullReset();
	virtual void FullReset_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="JumpingJack|Handler")
	void Update();
	virtual void Update_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void Add();
	virtual void Add_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void OnSubComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){};
};
