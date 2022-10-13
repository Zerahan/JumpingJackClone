// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/Characters/EnemyPawn_Basic_C.h"
#include "Components/SphereComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemyPawn_Basic_C::AEnemyPawn_Basic_C()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DamageTypeClass = UDamageType::StaticClass();

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CollisionCylinder"));
	if (CapsuleComponent) {
		CapsuleComponent->InitCapsuleSize(30.0f, 30.0f);
		CapsuleComponent->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);

		CapsuleComponent->CanCharacterStepUpOn = ECB_No;
		CapsuleComponent->SetShouldUpdatePhysicsVolume(true);
		CapsuleComponent->SetCanEverAffectNavigation(false);
		CapsuleComponent->bDynamicObstacle = true;
		RootComponent = CapsuleComponent;
		CapsuleComponent->SetCollisionProfileName("OverlapAllDynamic");
		CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &AEnemyPawn_Basic_C::OnSubComponentOverlap);
	}

	IsTeleporting	= false;
	FlyLeft			= true;
	FlySpeed		= 500.f;
	TeleportOffset	= 1;
	TeleportCollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("TeleportCollisionSphere"));
	if (TeleportCollisionSphere) {
		TeleportCollisionSphere->SetupAttachment(RootComponent);
		TeleportCollisionSphere->SetSphereRadius(5.f);
		TeleportCollisionSphere->SetCollisionProfileName("OverlapAllDynamic");
		TeleportCollisionSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	}

	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	if (Mesh) {
		Mesh->SetupAttachment(RootComponent);
		Mesh->SetCollisionProfileName("NoCollision");
	}
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	if (ProjectileMovement) {
		ProjectileMovement->ProjectileGravityScale = 0;
		ProjectileMovement->Velocity		= FVector::RightVector * (FlyLeft ? 1 : -1);
		ProjectileMovement->MaxSpeed		= FlySpeed;
		ProjectileMovement->InitialSpeed	= FlySpeed;
	}
}

// Called when the game starts or when spawned
void AEnemyPawn_Basic_C::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyPawn_Basic_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AEnemyPawn_Basic_C::GetFlyLeft() const { return FlyLeft; }

void AEnemyPawn_Basic_C::SetFlyLeft_Implementation(bool NewFlyLeft)
{
	if(FlyLeft == NewFlyLeft) return;
	FlyLeft = NewFlyLeft;
	if (ProjectileMovement) {
		ProjectileMovement->Velocity = FVector::RightVector * (FlyLeft ? 1 : -1);
	}
}

int AEnemyPawn_Basic_C::GetTeleportOffset_Implementation() const
{
	return TeleportOffset;
}

void AEnemyPawn_Basic_C::TeleportTo_Implementation(FTransform TargetTransform)
{
	IsTeleporting = true;
	SetActorLocation(TargetTransform.GetLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	IsTeleporting = false;
}

bool AEnemyPawn_Basic_C::GetIsTeleporting_Implementation() const
{
	return IsTeleporting;
}

void AEnemyPawn_Basic_C::SetIsTeleporting_Implementation(bool NewIsTeleporting)
{
	if (IsTeleporting == NewIsTeleporting) return;
	IsTeleporting = NewIsTeleporting;
}

float AEnemyPawn_Basic_C::GetEntityHalfWidth_Implementation() const
{
	return TeleportCollisionSphere->GetScaledSphereRadius();
}

bool AEnemyPawn_Basic_C::IsTeleportCollider_Implementation(UPrimitiveComponent* ComponentToCheck) const
{
	return TeleportCollisionSphere == ComponentToCheck;
}

void AEnemyPawn_Basic_C::OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this) return;
	//if (!OtherActor->GetClass()->IsChildOf<APawn>()) return;
	UGameplayStatics::ApplyDamage(OtherActor, 1, GetController(), this, DamageTypeClass);
}

