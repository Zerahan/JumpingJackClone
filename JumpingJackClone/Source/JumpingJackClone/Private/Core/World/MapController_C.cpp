// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/World/MapController_C.h"
#include "Core/World/Handler_Basic_C.h"
#include "Core/World/Handler_Platform_C.h"
#include "Core/Characters/CharacterBasic_C.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Game/JumpingJack_GM_C.h"

// Sets default values
AMapController_C::AMapController_C()
	:Super()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Width = 1000;
	Height = 1000;
	PlatformCount = 8;
	WallThickness = 100;

	ConstructorHelpers::FObjectFinder<UStaticMesh> MeshObj(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultRootObject"));

	BottomMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bottom Mesh Boundary"));
	if (BottomMesh) {
		BottomMesh->SetupAttachment(RootComponent);
		BottomMesh->SetRelativeLocation(FVector(0, 0, -(WallThickness / 2)));
		BottomMesh->SetNotifyRigidBodyCollision(true);
	}

	TopMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top Mesh Boundary"));
	if (TopMesh) {
		TopMesh->SetupAttachment(RootComponent);
		TopMesh->SetRelativeLocation(FVector(0, 0, Height + (WallThickness / 2)));
		TopMesh->SetNotifyRigidBodyCollision(true);
	}

	LeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Mesh Boundary"));
	if (LeftMesh) {
		LeftMesh->SetupAttachment(RootComponent);
		LeftMesh->SetRelativeLocation(FVector(0, (WallThickness / 2) - Width / 2, Height / 2));
		LeftMesh->SetNotifyRigidBodyCollision(true);
	}

	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Mesh Boundary"));
	if (RightMesh) {
		RightMesh->SetupAttachment(RootComponent);
		RightMesh->SetRelativeLocation(FVector(0, (WallThickness / 2) + Width / 2, Height / 2));
		RightMesh->SetNotifyRigidBodyCollision(true);
	}

	BackMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Back Mesh"));
	if (BackMesh) {
		BackMesh->SetupAttachment(RootComponent);
		BackMesh->SetRelativeLocation(FVector(-60, 0, Height / 2));
		BackMesh->SetNotifyRigidBodyCollision(true);
	}
	
	if (MeshObj.Succeeded()) {
		FVector MeshSize = MeshObj.Object->GetBoundingBox().GetSize();
		BottomMesh->SetStaticMesh(MeshObj.Object);
		TopMesh->SetStaticMesh(MeshObj.Object);
		LeftMesh->SetStaticMesh(MeshObj.Object);
		RightMesh->SetStaticMesh(MeshObj.Object);
		BackMesh->SetStaticMesh(MeshObj.Object);

		BottomMesh->SetRelativeScale3D(FVector(1, (Width + (WallThickness*2)) / MeshSize.Y, WallThickness / MeshSize.Z));
		TopMesh->SetRelativeScale3D(FVector(1, (Width + (WallThickness * 2)) / MeshSize.Y, WallThickness / MeshSize.Z));
		LeftMesh->SetRelativeScale3D(FVector(1, WallThickness / MeshSize.Y, Height / MeshSize.Z));
		RightMesh->SetRelativeScale3D(FVector(1, WallThickness / MeshSize.Y, Height / MeshSize.Z));
		BackMesh->SetRelativeScale3D(FVector(20 / MeshSize.X, (Width + (WallThickness * 2)) / MeshSize.Y, (Height + (WallThickness * 2)) / MeshSize.Z));
	}
}

// Called when the game starts or when spawned
void AMapController_C::BeginPlay()
{
	Super::BeginPlay();
	FVector MeshSize = BottomMesh->GetStaticMesh()->GetBoundingBox().GetSize();
	BottomMesh->SetRelativeScale3D(FVector(1, (Width + (WallThickness * 2)) / MeshSize.Y, WallThickness / MeshSize.Z));

	MeshSize = TopMesh->GetStaticMesh()->GetBoundingBox().GetSize();
	TopMesh->SetRelativeScale3D(FVector(1, (Width+ (WallThickness * 2)) / MeshSize.Y, WallThickness / MeshSize.Z));
	TopMesh->SetRelativeLocation(FVector(TopMesh->GetRelativeLocation().X, TopMesh->GetRelativeLocation().Y, Height + (WallThickness / 2)));

	MeshSize = LeftMesh->GetStaticMesh()->GetBoundingBox().GetSize();
	LeftMesh->SetRelativeScale3D(FVector(1, WallThickness / MeshSize.Y, Height / MeshSize.Z));
	LeftMesh->SetRelativeLocation(FVector(0, -(WallThickness / 2) - Width / 2, Height / 2));
	
	MeshSize = RightMesh->GetStaticMesh()->GetBoundingBox().GetSize();
	RightMesh->SetRelativeScale3D(FVector(1, WallThickness / MeshSize.Y, Height / MeshSize.Z));
	RightMesh->SetRelativeLocation(FVector(0, (WallThickness / 2) + Width / 2, Height / 2));
	
	MeshSize = BackMesh->GetStaticMesh()->GetBoundingBox().GetSize();
	BackMesh->SetRelativeScale3D(FVector(20 / MeshSize.X, (Width + (WallThickness * 2)) / MeshSize.Y, (Height + (WallThickness * 2)) / MeshSize.Z));
	BackMesh->SetRelativeLocation(FVector(-50 - (MeshSize.X / 2), 0, Height / 2));
	
	TArray<UHandler_Basic_C*> FoundHandlers;
	GetComponents<UHandler_Basic_C>(FoundHandlers);
	for (UHandler_Basic_C* Handler : FoundHandlers) {
		if (Handler) {
			Handler->InjectMapSize(Width, Height, PlatformCount);
			Handler->Reset(true);
			Handler->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
			HandlerRefs.Add(Handler);
		}
	}

	Reset();

	AJumpingJack_GM_C* Gamemode = Cast<AJumpingJack_GM_C>(GetWorld()->GetAuthGameMode());
	Gamemode->RegisterMapController(this);
}

// Called every frame
void AMapController_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FVector AMapController_C::GetTeleportTargetLocation_Implementation(FVector ActorLocation, uint8 RowOffset)
{
	FVector NewLocation = ActorLocation;
	NewLocation.Y = NewLocation.Y * -1;
	if (RowOffset == 0) {
		return NewLocation;
	}
	else {
		UHandler_Platform_C* PlatformHandler;
		if (HandlerRefs.FindItemByClass<UHandler_Platform_C>(&PlatformHandler)) {
			NewLocation.Z = NewLocation.Z + (RowOffset * PlatformHandler->GetSpaceBetweenPlatforms());
			if (NewLocation.Z > (GetActorLocation().Z + Height)) {
				NewLocation.Z -= Height;
			}
			else if (NewLocation.Z < (GetActorLocation().Z)) {
				NewLocation.Z += Height;
			}
		}
		return NewLocation;
	}
	return ActorLocation;
}

bool AMapController_C::IsVictoryComponent(UPrimitiveComponent* Component)
{
	return Component == TopMesh;
}

bool AMapController_C::IsDamageComponent(UPrimitiveComponent* Component)
{
	return Component == BottomMesh;
}

void AMapController_C::SetDifficultyLevel_Implementation(int NewDifficultyLevel)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Difficulty Level is now: %i"), NewDifficultyLevel));
	for (auto Handler : HandlerRefs) {
		Handler->SetDifficultyLevel(NewDifficultyLevel);
	}
}

void AMapController_C::Reset() {
	Super::Reset();
	for ( UHandler_Basic_C* Handler : HandlerRefs ) {
		Handler->Reset();
	}

	APlayerController* LocalPlayer = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (IsValid(LocalPlayer)) {
		ACharacterBasic_C* PlayerCharacter = Cast<ACharacterBasic_C>(LocalPlayer->GetPawn());
		if (IsValid(PlayerCharacter)) {
			FVector Loc = GetActorLocation();
			Loc.Z += Height / 2;
			PlayerCharacter->SetCameraLocation(Loc);
			PlayerCharacter->SetCameraWidth((Width > Height ? Width : Height));
		}
	}
}

#if WITH_EDITOR
void AMapController_C::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}
#endif

