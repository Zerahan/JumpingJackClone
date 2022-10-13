// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/CharacterBasic_C.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Core/World/Handler_Basic_C.h"
#include "Core/World/MapController_C.h"
#include "Core/Game/JumpingJack_GM_C.h"
#include "Core/Game/CorePlayerState_C.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Game/DamageTypes/DamageType_Fall.h"

// Sets default values
ACharacterBasic_C::ACharacterBasic_C()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;
	GetCapsuleComponent()->SetCapsuleRadius(5.f);
	GetCapsuleComponent()->ComponentTags.Add("TeleportCollision");
	
	IsInvulnerable = false;
	CanTakeFallDamage = false;
	DamageRecoveryDurationInSeconds = 2;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	if (CameraArm) {
		CameraArm->SetupAttachment(GetCapsuleComponent());
		CameraArm->bDoCollisionTest = false;
		CameraArm->TargetArmLength = 2000;
		CameraArm->SetUsingAbsoluteLocation(true);
		CameraArm->SetUsingAbsoluteRotation(true);
		CameraArm->SetWorldRotation(FRotator(0, 180, 0));
		CameraArm->SetRelativeLocation(FVector(0.f, 0.f, 0.f));
	}
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	if (Camera) {
		if (CameraArm) {
			Camera->SetupAttachment(CameraArm);
		}
		Camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
		Camera->SetOrthoWidth(512*4);
		//Camera->SetOrthoFarClipPlane(Camera->OrthoFarClipPlane);
		//Camera->SetOrthoNearClipPlane(Camera->OrthoNearClipPlane);
	}
	
	if (GetMesh()) {
		GetMesh()->SetCollisionProfileName("NoCollision");
	}

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACharacterBasic_C::OnSubComponentHit);

	GetCharacterMovement()->JumpZVelocity = 420.f;
}

// Called when the game starts or when spawned
void ACharacterBasic_C::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACharacterBasic_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Unstuck();
}

void ACharacterBasic_C::SetCameraLocation(FVector NewLocation) {
	if (IsValid(CameraArm)) {
		CameraArm->SetWorldLocation(FVector(0, NewLocation.Y, NewLocation.Z));
	}
}

void ACharacterBasic_C::SetCameraWidth(float NewWidth)
{
	if(CameraWidth == NewWidth) return;
	CameraWidth = NewWidth*2;
	if (Camera) {
		Camera->OrthoWidth = CameraWidth;
	}
}

bool ACharacterBasic_C::GetIsRecoveringFromDamage() const { return IsRecoveringFromDamage; }

void ACharacterBasic_C::SetIsRecoveringFromDamage(bool NewIsRecoveringFromDamage) {
	if (IsRecoveringFromDamage == NewIsRecoveringFromDamage) return;
	IsRecoveringFromDamage = NewIsRecoveringFromDamage;
	GetController()->SetIgnoreMoveInput(IsRecoveringFromDamage);
	OnRecoveringFromDamage.Broadcast(IsRecoveringFromDamage);
}

void ACharacterBasic_C::OnDamageRecoveryTimerFinished_Implementation()
{
	SetIsRecoveringFromDamage(false);
	GetWorld()->GetTimerManager().ClearTimer(DamageRecoveryTimer);
}

// Called to bind functionality to input
void ACharacterBasic_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacterBasic_C::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAxis("MoveRight", this, &ACharacterBasic_C::MoveRight);
}

void ACharacterBasic_C::PossessedBy(AController* NewController) {
	Super::PossessedBy(NewController);
	PlayerStateRef = NewController->GetPlayerState<ACorePlayerState_C>();
}

void ACharacterBasic_C::UnPossessed() {
	Super::UnPossessed();
	PlayerStateRef = nullptr;
}

void ACharacterBasic_C::Landed(const FHitResult& Hit)
{
	CanTakeFallDamage = true;
}

void ACharacterBasic_C::MoveRight_Implementation(float AxisValue) {
	if (!GetCharacterMovement()->IsFalling()) {
		AddMovementInput(CameraArm->GetRightVector(), AxisValue);
	}
}

void ACharacterBasic_C::Unstuck()
{
	FHitResult Hit;
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + (FVector::DownVector * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight()-1)), ECollisionChannel::ECC_Visibility, FCollisionQueryParams(), FCollisionResponseParams());
	//DrawDebugLine(GetWorld(), Hit.TraceStart + (FVector::ForwardVector * 50), Hit.TraceEnd + (FVector::ForwardVector * 50), FColor::Red, false, 0, 0, 4.f);
	if (Hit.IsValidBlockingHit()) {
		IsInvulnerable = true;
		float HitOffset = FVector::Distance(Hit.TraceStart, Hit.TraceEnd) - Hit.Distance;
		SetActorLocation(GetActorLocation() + FVector::UpVector * HitOffset, false, nullptr, ETeleportType::ResetPhysics);
		IsInvulnerable = false;
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Moving UP by %i!"), (int)(FVector::Distance(Hit.TraceStart, Hit.TraceEnd) - Hit.Distance)));
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Collision!")));
		//DrawDebugBox(GetWorld(), Hit.ImpactPoint + (FVector::ForwardVector * 50), FVector(0.1f, 0.1f, 0.1f), FColor::Red, false, 1, 0, 4.f);
	}
	else {
		GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation() + (FVector::UpVector * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 1)), ECollisionChannel::ECC_Visibility, FCollisionQueryParams(), FCollisionResponseParams());
		if (Hit.IsValidBlockingHit()) {
			float HitOffset = FVector::Distance(Hit.TraceStart, Hit.TraceEnd) - Hit.Distance;
			IsInvulnerable = true;
			SetActorLocation(GetActorLocation() + FVector::DownVector * HitOffset, false, nullptr, ETeleportType::ResetPhysics);
			IsInvulnerable = false;
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Moving DOWN by %i!"), (int)(FVector::Distance(Hit.TraceStart, Hit.TraceEnd) - Hit.Distance)));
			//DrawDebugLine(GetWorld(), Hit.TraceStart + (FVector::ForwardVector * 50), Hit.TraceEnd + (FVector::ForwardVector * 50), FColor::Red, false, 0, 0, 4.f);
			//DrawDebugBox(GetWorld(), Hit.ImpactPoint + (FVector::ForwardVector * 50), FVector(0.1f, 0.1f, 0.1f), FColor::Red, false, 1, 0, 4.f);
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Collision!")));
		}
	}
	if (GetActorLocation().Z < GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - 1) {
		FVector Location = GetActorLocation();
		Location.Z = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		SetActorLocation(Location);
	}
	if (!FMath::IsNearlyEqual(GetActorLocation().X, 0.f, 0.1f)) {
		FVector Location = GetActorLocation();
		Location.X = 0;
		SetActorLocation(Location);
	}
}

void ACharacterBasic_C::Jump()
{
	if (GetIsRecoveringFromDamage()) return;
	if (!CanJump()) return;
	Super::Jump();
	CanTakeFallDamage = false;
	ConsumeMovementInputVector();
	GetCharacterMovement()->Velocity = FVector();
}

float ACharacterBasic_C::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	//if (DamageCauser) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("DamageCauser! %s"), *DamageCauser->GetName()));
	float DamageRemaining = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	if (IsInvulnerable) return DamageRemaining;
	if (!CanTakeFallDamage && DamageEvent.DamageTypeClass.Get()->IsChildOf<UDamageType_Fall>()) return DamageRemaining;
	SetIsRecoveringFromDamage(true);
	GetWorld()->GetTimerManager().SetTimer(DamageRecoveryTimer, this, &ACharacterBasic_C::OnDamageRecoveryTimerFinished, DamageRecoveryDurationInSeconds);
	DamageRemaining = 0;
	return DamageRemaining;
}

int ACharacterBasic_C::GetTeleportOffset_Implementation() const
{
	return 0;
}

void ACharacterBasic_C::TeleportTo_Implementation(FTransform TargetTransform)
{
	IsTeleporting = true;
	SetActorLocation(TargetTransform.GetLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	IsTeleporting = false;
	//SetActorRotation(TargetTransform.GetRotation());
}

bool ACharacterBasic_C::GetIsTeleporting_Implementation() const
{
	return IsTeleporting;
}

void ACharacterBasic_C::SetIsTeleporting_Implementation(bool NewIsTeleporting)
{
	if (IsTeleporting == NewIsTeleporting) return;
	IsTeleporting = NewIsTeleporting;
}

float ACharacterBasic_C::GetEntityHalfWidth_Implementation() const
{
	return GetCapsuleComponent()->GetScaledCapsuleRadius() + 0.1;
}

bool ACharacterBasic_C::IsTeleportCollider_Implementation(UPrimitiveComponent* ComponentToCheck) const
{
	return ComponentToCheck == GetCapsuleComponent();
}

void ACharacterBasic_C::OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
}

void ACharacterBasic_C::OnSubComponentHit_Implementation(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor == this) return;
	AMapController_C* MapController = Cast<AMapController_C>(OtherActor);
	if (IsValid(MapController)) {
		AJumpingJack_GM_C* Gamemode = Cast<AJumpingJack_GM_C>(GetWorld()->GetAuthGameMode());
		if (!IsValid(Gamemode)) return;
		if (MapController->IsVictoryComponent(OtherComp)) {
			Gamemode->TriggerWin();
			return;
		}
		if (MapController->IsDamageComponent(OtherComp)) {
			if (IsInvulnerable) return;
			if (!CanTakeFallDamage) return;
			if (PlayerStateRef) {
				PlayerStateRef->SetLivesRemaining(-1);
				//CanTakeFallDamage = false;
				if (PlayerStateRef->GetLivesRemaining() == 0) {
					Gamemode->TriggerLose();
				}
			}
			return;
		}
		if (Hit.Normal.Z < -0.5f) {
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Hit.Normal.Z! %f < %f"), FMath::RoundFromZero(Hit.Normal.Z), -0.5f));
			UGameplayStatics::ApplyDamage(this, 1, nullptr, OtherActor, UDamageType::StaticClass());
		}
	}
}

