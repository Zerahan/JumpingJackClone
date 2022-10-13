// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/World/Handler_Platform_C.h"
#include "Core/World/Handler_Basic_C.h"
#include "Components/BoxComponent.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Core/Interfaces/TeleportInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Game/JumpingJack_GM_C.h"

UHandler_Platform_C::UHandler_Platform_C()
	:Super()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.TickInterval = 0.02;
	
	PlatformResolution	= 50;
	InitialHoleCount	= 2;
	HoleWidth			= 5;
	HoleMoveSpeed		= 1;
	DamageTypeClass		= UDamageType::StaticClass();
}

void UHandler_Platform_C::InjectMapSize(const int& _Width, const int& _Height, const uint8& _PlatformCount)
{
	Super::InjectMapSize(_Width, _Height, _PlatformCount);
	if (LeftTeleporter) {
		LeftTeleporter->SetBoxExtent(FVector(50.f, 5.f, MapHeight / 2));
		LeftTeleporter->SetRelativeLocation(FVector(0, MapWidth, MapHeight / 2));
	}
	if (RightTeleporter) {
		RightTeleporter->SetBoxExtent(FVector(50.f, 5.f, MapHeight / 2));
		RightTeleporter->SetRelativeLocation(FVector(0, 0, MapWidth / 2));
	}
}

void UHandler_Platform_C::BeginPlay()
{
	Super::BeginPlay();
}

void UHandler_Platform_C::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	for (int y = 0; y < PlatformCount; y++) {
		for (int x = 0; x < PlatformResolution; x++) {
			SetInstanceEnabled(x, y, true);
		}
	}
	
	for (int i = 0; i < ActiveHoles.Num(); i++) {
		FVector2D Location = ActiveHoles[i].Location;
		for (int x = 0; x < ActiveHoles[i].Width; x++) {
			SetInstanceEnabled(Location.X + x, Location.Y, true);
		}
		ActiveHoles[i].Tick(PlatformResolution, PlatformCount);
		for (int x = 0; x < ActiveHoles[i].Width; x++) {
			SetInstanceEnabled(Location.X + x, Location.Y, false);
		}
	}
}

float UHandler_Platform_C::GetSpaceBetweenPlatforms() const { return SpaceBetweenPlatforms; }

void UHandler_Platform_C::Reset_Implementation(bool ForceFullReset)
{
	Super::Reset_Implementation(ForceFullReset);
	for (int i = 0; i < PassthroughCollisionBoxes.Num(); i++) {
		if (IsValid(PassthroughCollisionBoxes[i])) {
			HasPassthroughSpawnedHole[i] = false;
			//PassthroughCollisionBoxes[i]->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		}
	}

	FVector2D RandomLocation = FVector2D(FMath::RandRange(0, PlatformResolution), FMath::RandRange(0, PlatformCount));
	//HoleWidth = FMath::Floor(PlatformResolution * 0.1f);
	ActiveHoles.Empty();
	for (int i = 0; i < InitialHoleCount; i++) {
		ActiveHoles.Add(FHoleData(RandomLocation, (ActiveHoles.IsValidIndex(i - 1) ? !ActiveHoles[i - 1].MoveDirection : true), HoleWidth));
	}
	for (int y = 0; y < PlatformCount; y++) {
		for (int x = 0; x < PlatformResolution; x++) {
			SetInstanceEnabled(x, y, true);
		}
	}
}

void UHandler_Platform_C::FullReset_Implementation()
{
	Super::FullReset_Implementation();
	SetRelativeLocation(FVector(0, -MapWidth/2, 0));
	if (PlatformCount != PlatformMeshes.Num()){
		PlatformMeshes.Empty();
		PassthroughCollisionBoxes.Empty();
	}
	else if(PlatformMeshes.IsValidIndex(0) && PlatformResolution != PlatformMeshes[0]->GetInstanceCount()) {
		for (auto M : PlatformMeshes) {
			M->ClearInstances();
		}
	}

	if (!IsValid(LeftTeleporter)) {
		LeftTeleporter = NewObject<UBoxComponent>(this);
		LeftTeleporter->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		LeftTeleporter->RegisterComponent();
		LeftTeleporter->SetBoxExtent(FVector(50.f, 5.f, MapHeight / 2));
		LeftTeleporter->SetRelativeLocation(FVector(0, MapWidth, MapHeight / 2));
		LeftTeleporter->SetRelativeRotation(FRotator(0, 0, 0));
		LeftTeleporter->SetCollisionProfileName("OverlapAllDynamic");
		LeftTeleporter->OnComponentBeginOverlap.AddDynamic(this, &UHandler_Platform_C::OnSubComponentOverlap);
	}
	if (!IsValid(RightTeleporter)) {
		RightTeleporter = NewObject<UBoxComponent>(this);
		RightTeleporter->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
		RightTeleporter->RegisterComponent();
		RightTeleporter->SetBoxExtent(FVector(50.f, 5.f, MapHeight / 2));
		RightTeleporter->SetRelativeLocation(FVector(0, 0, MapHeight / 2));
		RightTeleporter->SetRelativeRotation(FRotator(0, 0, 0));
		RightTeleporter->SetCollisionProfileName("OverlapAllDynamic");
		RightTeleporter->OnComponentBeginOverlap.AddDynamic(this, &UHandler_Platform_C::OnSubComponentOverlap);
	}

	UHierarchicalInstancedStaticMeshComponent* Mesh = nullptr;
	UBoxComponent* PassthroughCollider = nullptr;
	float OffsetValue = MapWidth / PlatformResolution;
	FTransform NewInstanceTransform = FTransform();
	for (int y = 0; y < PlatformCount; y++) {
		if (!PlatformMeshes.IsValidIndex(y) || !IsValid(PlatformMeshes[y])) {
			Mesh = NewObject<UHierarchicalInstancedStaticMeshComponent>(this);
			Mesh->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			Mesh->RegisterComponent();
			Mesh->SetStaticMesh(PlatformSubcomponentMesh);
			
			Mesh->SetMaterial(0, PlatformMaterial);
			Mesh->SetRelativeLocation(FVector(0, 0, (y+1) * (MapHeight / PlatformCount) - (5)));
			Mesh->SetRelativeRotation(FRotator(0, 0, 0));
			if (!PlatformMeshes.IsValidIndex(y)) {
				PlatformMeshes.Add(Mesh);
			} else {
				PlatformMeshes[y] = Mesh;
			}
			//PlatformMeshes.EmplaceAt_GetRef(y, Mesh);
			
			PassthroughCollider = NewObject<UBoxComponent>(this);
			PassthroughCollider->AttachToComponent(this, FAttachmentTransformRules::SnapToTargetIncludingScale);
			PassthroughCollider->RegisterComponent();
			PassthroughCollider->SetBoxExtent(FVector(50, MapWidth / 2, 2), false);
			PassthroughCollider->SetRelativeLocation(FVector(0, MapWidth/2, (y + 1) * SpaceBetweenPlatforms));
			PassthroughCollider->SetRelativeRotation(FRotator(0, 0, 0));
			PassthroughCollider->SetCollisionProfileName("OverlapAllDynamic");
			PassthroughCollider->OnComponentBeginOverlap.AddDynamic(this, &UHandler_Platform_C::OnSubComponentOverlap);
			
			if (!PassthroughCollisionBoxes.IsValidIndex(y)) {
				PassthroughCollisionBoxes.Add(PassthroughCollider);
				HasPassthroughSpawnedHole.Add(false);
			}
			else {
				PassthroughCollisionBoxes[y] = PassthroughCollider;
				HasPassthroughSpawnedHole[y] = false;
			}

		} else {
			Mesh = PlatformMeshes[y];
		}
		NewInstanceTransform = FTransform(FRotator(0, 0, 0), FVector(0, OffsetValue/2,0), FVector(1, (OffsetValue / 100), 0.2f));
		for (int x = 0; x < PlatformResolution; x++) {
			Mesh->AddInstance(NewInstanceTransform);
			NewInstanceTransform.SetLocation(NewInstanceTransform.GetLocation() + FVector(0, OffsetValue, 0));
		}
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Res %i"), Mesh->GetInstanceCount()));
	}
}

void UHandler_Platform_C::Update_Implementation()
{
	Super::Update_Implementation();
}

void UHandler_Platform_C::Add_Implementation()
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Add hole?")));
	Super::Add_Implementation();
	
	FVector2D RandomLocation = FVector2D(FMath::RandRange(0, PlatformResolution), FMath::RandRange(0, PlatformCount));
	ActiveHoles.Add(FHoleData(RandomLocation, (ActiveHoles.Num() > 1 ? !ActiveHoles[ActiveHoles.Num() - 1].MoveDirection : true), HoleWidth));
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Hole added! %i"), ActiveHoles.Num()));
}

void UHandler_Platform_C::OnSubComponentOverlap_Implementation(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Collision!")));
	//if (OverlappedComponent) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("OverlappedComponent! %s"), *OverlappedComponent->GetName()));
	//if (OtherComp) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("OtherComp! %s"), *OtherComp->GetName()));
	//if (OtherActor) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("OtherActor! %s"), *OtherActor->GetName()));
	//if (GetOwner()) GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Owner! %s"), *GetOwner()->GetName()));
	if (OtherActor == GetOwner()) return;
	//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("OtherActor != GetOwner!")));
	if (OverlappedComponent == LeftTeleporter) {
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Left Teleporter!")));
		ITeleportInterface* TeleportActor = Cast<ITeleportInterface>(OtherActor);
		if (TeleportActor) {
			if (!TeleportActor->Execute_IsTeleportCollider(OtherActor, OtherComp)) return;
			//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Left Teleporter: Cast!")));
			if (!TeleportActor->Execute_GetIsTeleporting(OtherActor)) {
				//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Left Teleporter: Not currently teleporting!")));
				FVector NewLocation = OtherActor->GetActorLocation();
				NewLocation.Y = RightTeleporter->GetComponentLocation().Y + RightTeleporter->GetScaledBoxExtent().Y + TeleportActor->Execute_GetEntityHalfWidth(OtherActor);
				int Offset = TeleportActor->Execute_GetTeleportOffset(OtherActor);
				if (Offset != 0) {
					NewLocation.Z += Offset * SpaceBetweenPlatforms;
					if (NewLocation.Z > MapHeight) {
						NewLocation.Z -= (MapHeight - SpaceBetweenPlatforms);
					}
					else if(NewLocation.Z <= SpaceBetweenPlatforms) {
						NewLocation.Z += (MapHeight - SpaceBetweenPlatforms);
					}
				}
				TeleportActor->Execute_TeleportTo(OtherActor, FTransform(NewLocation));
			}
			else {
				TeleportActor->Execute_SetIsTeleporting(OtherActor, false);
			}
		}
		return;
	}
	if (OverlappedComponent == RightTeleporter) {
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Right Teleporter!")));
		ITeleportInterface* TeleportActor = Cast<ITeleportInterface>(OtherActor);
		if (TeleportActor) {
			if (!TeleportActor->Execute_IsTeleportCollider(OtherActor, OtherComp)) return;
			if (!TeleportActor->Execute_GetIsTeleporting(OtherActor)) {
				FVector NewLocation = OtherActor->GetActorLocation();
				NewLocation.Y = LeftTeleporter->GetComponentLocation().Y - LeftTeleporter->GetScaledBoxExtent().Y - TeleportActor->Execute_GetEntityHalfWidth(OtherActor);
				int Offset = TeleportActor->Execute_GetTeleportOffset(OtherActor);
				if (Offset != 0) {
					NewLocation.Z += Offset * SpaceBetweenPlatforms;
					if (NewLocation.Z > MapHeight) {
						NewLocation.Z -= (MapHeight - SpaceBetweenPlatforms);
					}
					else if (NewLocation.Z <= SpaceBetweenPlatforms) {
						NewLocation.Z += (MapHeight - SpaceBetweenPlatforms);
					}
				}
				TeleportActor->Execute_TeleportTo(OtherActor, FTransform(NewLocation));
			}
			else {
				TeleportActor->Execute_SetIsTeleporting(OtherActor, false);
			}
		}
		return;
	}
	int index = PassthroughCollisionBoxes.Find((UBoxComponent*)OverlappedComponent);
	if (index != INDEX_NONE) {
		if (!HasPassthroughSpawnedHole[index]) {
			//PassthroughCollisionBoxes[index]->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			HasPassthroughSpawnedHole[index] = true;
			Add();
		}
		if (OtherActor->GetActorLocation().Z > PassthroughCollisionBoxes[index]->GetComponentLocation().Z) {
			UGameplayStatics::ApplyDamage(OtherActor, 1, nullptr, GetOwner(), DamageTypeClass);
		}
		else {
			if (Cast<APawn>(OtherActor)) {
				AJumpingJack_GM_C* Gamemode = Cast<AJumpingJack_GM_C>(GetWorld()->GetAuthGameMode());
				if (Gamemode) {
					Gamemode->AddScore(((APawn*)OtherActor)->GetController(), 5);
				}
			}
		}
	}
}

void UHandler_Platform_C::SetInstanceEnabled_Implementation(uint8 RowIndex, uint8 ColumnIndex, bool Value)
{
	uint8 AdjustedRowIndex = RowIndex;
	uint8 AdjustedColumnIndex = ColumnIndex;

	if (AdjustedRowIndex < 0) {
		AdjustedRowIndex += PlatformResolution;
		AdjustedColumnIndex--;
	}
	else if (AdjustedRowIndex >= PlatformResolution) {
		AdjustedRowIndex -= PlatformResolution;
		AdjustedColumnIndex++;
	}

	if (AdjustedColumnIndex < 0) {
		AdjustedColumnIndex += PlatformCount;
	}
	else if (AdjustedColumnIndex >= PlatformCount) {
		AdjustedColumnIndex -= PlatformCount;
	}
	if (!PlatformMeshes.IsValidIndex(AdjustedColumnIndex)) return;
	if (PlatformMeshes[AdjustedColumnIndex]->GetInstanceCount() < AdjustedRowIndex) return;
	FTransform InstanceTransform;
	PlatformMeshes[AdjustedColumnIndex]->GetInstanceTransform(AdjustedRowIndex, InstanceTransform);
	FVector Location = InstanceTransform.GetLocation();
	Location.X = (Value ? 0 : -500);
	InstanceTransform.SetLocation(Location);
	PlatformMeshes[AdjustedColumnIndex]->UpdateInstanceTransform(AdjustedRowIndex, InstanceTransform, false);
}
