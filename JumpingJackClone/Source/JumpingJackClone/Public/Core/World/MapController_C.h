// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapController_C.generated.h"

class UHandler_Basic_C;
class UStaticMeshComponent;
class UBoxComponent;
class UPrimitiveComponent;

UCLASS(BlueprintType, Blueprintable, Abstract)
class JUMPINGJACKCLONE_API AMapController_C : public AActor
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, Category = "JumpingJack|Map", meta = (AllowPrivateAccess = "true"))
	TArray<UHandler_Basic_C*> HandlerRefs;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "JumpingJack|Map", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BottomMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "JumpingJack|Map", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* TopMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "JumpingJack|Map", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* LeftMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "JumpingJack|Map", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* RightMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "JumpingJack|Map", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* BackMesh;

	float WallThickness;
	
protected:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "JumpingJack|Map", meta = (BlueprintProtected))
	int Width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "JumpingJack|Map", meta = (BlueprintProtected))
	int Height;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "JumpingJack|Map", meta = (BlueprintProtected))
	int PlatformCount;
	
public:
	// Sets default values for this actor's properties
	AMapController_C();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Map")
	FVector GetTeleportTargetLocation(FVector ActorLocation, uint8 RowOffset = 0);
	virtual FVector GetTeleportTargetLocation_Implementation(FVector ActorLocation, uint8 RowOffset = 0);

	UFUNCTION(BlueprintCallable)
	bool IsVictoryComponent(UPrimitiveComponent* Component);
	bool IsDamageComponent(UPrimitiveComponent* Component);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetDifficultyLevel(int NewDifficultyLevel);
	virtual void SetDifficultyLevel_Implementation(int NewDifficultyLevel);

	virtual void Reset() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
};
