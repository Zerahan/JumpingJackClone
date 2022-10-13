// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Core/World/Handler_Basic_C.h"
#include "Handler_Platform_C.generated.h"

class UBoxComponent;
class UHierarchicalInstancedStaticMeshComponent;

USTRUCT(BlueprintType)
struct FHoleData {
	GENERATED_BODY()
	
	FHoleData()
		: Location(FVector2D()), MoveDirection(false), Width(5)
	{}
	FHoleData(FVector2D _Location, bool _MoveDirection, uint8 _Width = 5)
		: Location(_Location), MoveDirection(_MoveDirection), Width(_Width)
	{}
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector2D Location;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool MoveDirection;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	uint8 Width;

	void Tick(uint8 MapWidth, uint8 MapHeight) {
		Location.X = Location.X + (MoveDirection ? -1 : 1);
		if (Location.X < 0) {
			Location.X += MapWidth;
			Location.Y--;
		}
		else if(Location.X >= MapWidth) {
			Location.X -= MapWidth;
			Location.Y++;
		}
		
		if (Location.Y < 0) {
			Location.Y += MapHeight;
		}
		else if (Location.Y >= MapHeight) {
			Location.Y -= MapHeight;
		}
	}
};

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, Abstract)
class JUMPINGJACKCLONE_API UHandler_Platform_C : public UHandler_Basic_C
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	uint8 PlatformResolution;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	uint8 InitialHoleCount;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	uint8 HoleWidth;
	
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float HoleMoveSpeed;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMesh* PlatformSubcomponentMesh;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* LeftTeleporter;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* RightTeleporter;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<FHoleData> ActiveHoles;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UHierarchicalInstancedStaticMeshComponent*> PlatformMeshes;
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	TArray<UBoxComponent*> PassthroughCollisionBoxes;

	UPROPERTY()
	TArray<bool> HasPassthroughSpawnedHole;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageTypeClass;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* PlatformMaterial;
	
public:
	UHandler_Platform_C();

	virtual void InjectMapSize(const int& _Width, const int& _Height, const uint8& _PlatformCount) override;

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "JumpingJack|Handler")
	float GetSpaceBetweenPlatforms() const;
	
	virtual void Reset_Implementation(bool ForceFullReset = true) override;
	virtual void FullReset_Implementation() override;
	virtual void Update_Implementation() override;
	virtual void Add_Implementation() override;

	virtual void OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void SetInstanceEnabled(uint8 RowIndex, uint8 ColumnIndex, bool Value);
	virtual void SetInstanceEnabled_Implementation(uint8 RowIndex, uint8 ColumnIndex, bool Value);
};
