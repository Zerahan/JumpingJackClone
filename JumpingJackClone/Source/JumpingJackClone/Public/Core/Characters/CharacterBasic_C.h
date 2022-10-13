// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Core/Interfaces/TeleportInterface.h"
#include "CharacterBasic_C.generated.h"

class USpringArmComponent;
class UBoxComponent;
class UCameraComponent;
class AMapController_C;
class ACorePlayerState_C;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnRecoveringFromDamage, bool, IsRecovering);

UCLASS(BlueprintType, Blueprintable, Abstract)
class JUMPINGJACKCLONE_API ACharacterBasic_C : public ACharacter, public ITeleportInterface
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Camera", meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Core|Camera", meta = (AllowPrivateAccess = "true"))
	ACorePlayerState_C* PlayerStateRef;

	UPROPERTY()
	float CameraWidth;
	
	UPROPERTY()
	bool IsTeleporting;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, meta=(AllowPrivateAccess="true"))
	bool IsInvulnerable;

	UPROPERTY(BlueprintReadWrite, VisibleInstanceOnly, meta = (AllowPrivateAccess = "true"))
	bool CanTakeFallDamage;

	UPROPERTY()
	bool IsRecoveringFromDamage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta = (AllowPrivateAccess = "true"))
	float DamageRecoveryDurationInSeconds;

	UPROPERTY()
	FTimerHandle DamageRecoveryTimer;

public:
	// Sets default values for this character's properties
	ACharacterBasic_C();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetCameraLocation(FVector NewLocation);

	UFUNCTION(BlueprintCallable)
	void SetCameraWidth(float NewWidth);

	UFUNCTION(BlueprintCallable)
	bool GetIsRecoveringFromDamage() const;

	UFUNCTION(BlueprintCallable)
	void SetIsRecoveringFromDamage(bool NewIsRecoveringFromDamage);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDamageRecoveryTimerFinished();
	virtual void OnDamageRecoveryTimerFinished_Implementation();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PossessedBy(AController* NewController) override;
	virtual void UnPossessed();

	virtual void Landed(const FHitResult& Hit) override;
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void MoveRight(float AxisValue);
	virtual void MoveRight_Implementation(float AxisValue);

	UFUNCTION(BlueprintCallable)
	void Unstuck();

	virtual void Jump() override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	
	virtual int GetTeleportOffset_Implementation() const override;
	virtual void TeleportTo_Implementation(FTransform TargetTransform) override;
	virtual bool GetIsTeleporting_Implementation() const override;
	virtual void SetIsTeleporting_Implementation(bool NewIsTeleporting) override;
	virtual float GetEntityHalfWidth_Implementation() const override;
	virtual bool IsTeleportCollider_Implementation(UPrimitiveComponent* ComponentToCheck) const override;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void OnSubComponentOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	virtual void OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "JumpingJack|Handler")
	void OnSubComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	virtual void OnSubComponentHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UPROPERTY(BlueprintAssignable)
	FOnRecoveringFromDamage OnRecoveringFromDamage;
};
