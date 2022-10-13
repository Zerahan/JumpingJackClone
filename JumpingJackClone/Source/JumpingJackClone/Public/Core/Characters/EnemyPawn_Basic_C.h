// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Core/Interfaces/TeleportInterface.h"
#include "EnemyPawn_Basic_C.generated.h"

class USphereComponent;
class USkeletalMeshComponent;
class UProjectileMovementComponent;
class UCapsuleComponent;

UCLASS(Blueprintable, BlueprintType, Abstract)
class JUMPINGJACKCLONE_API AEnemyPawn_Basic_C : public APawn, public ITeleportInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Camera", meta = (AllowPrivateAccess = "true"))
	USphereComponent* TeleportCollisionSphere;

	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Camera", meta = (AllowPrivateAccess = "true"))
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = Pawn, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY()
	bool FlyLeft;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	float FlySpeed;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	int TeleportOffset;

	UPROPERTY()
	bool IsTeleporting;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UDamageType> DamageTypeClass;

public:
	// Sets default values for this pawn's properties
	AEnemyPawn_Basic_C();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool GetFlyLeft() const;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetFlyLeft(bool NewFlyLeft = true);
	virtual void SetFlyLeft_Implementation(bool NewFlyLeft = true);

	virtual int GetTeleportOffset_Implementation() const override;
	virtual void TeleportTo_Implementation(FTransform TargetTransform) override;
	virtual bool GetIsTeleporting_Implementation() const override;
	virtual void SetIsTeleporting_Implementation(bool NewIsTeleporting) override;
	virtual float GetEntityHalfWidth_Implementation() const override;
	virtual bool IsTeleportCollider_Implementation(UPrimitiveComponent* ComponentToCheck) const override;


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void OnSubComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
