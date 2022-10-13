// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/World/Handler_Enemy_C.h"
#include "Core/World/Handler_Basic_C.h"
#include "Core/World/Handler_Platform_C.h"
#include "Core/Characters/EnemyPawn_Basic_C.h"

UHandler_Enemy_C::UHandler_Enemy_C()
	:Super()
{
	EnemyTypes.Add(AEnemyPawn_Basic_C::StaticClass());
}

void UHandler_Enemy_C::SetDifficultyLevel_Implementation(int NewDifficultyLevel)
{
	if (NewDifficultyLevel == SpawnedEnemies.Num()) return;
	int NumToSpawn = NewDifficultyLevel - SpawnedEnemies.Num();
	if(NumToSpawn > 0){
		for (int i = 0; i < NumToSpawn; ++i) {
			Add();
		}
	}
}
 
void UHandler_Enemy_C::InjectMapSize(const int& _Width, const int& _Height, const uint8& _PlatformCount)
{
	Super::InjectMapSize(_Width, _Height, _PlatformCount);
}

void UHandler_Enemy_C::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHandler_Enemy_C::BeginPlay()
{
	Super::BeginPlay();
}

void UHandler_Enemy_C::Reset_Implementation(bool ForceFullReset)
{
	Super::Reset_Implementation(ForceFullReset);
	for (auto Enemy : SpawnedEnemies) {
		if (IsValid(Enemy)) {
			Enemy->Destroy();
		}
	}
	SpawnedEnemies.Empty();
}

void UHandler_Enemy_C::FullReset_Implementation()
{
	Super::FullReset_Implementation();
}

void UHandler_Enemy_C::Update_Implementation()
{
	Super::Update_Implementation();
}

void UHandler_Enemy_C::Add_Implementation()
{
	Super::Add_Implementation();
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	int SelectedEnemyType = FMath::RandRange(0, EnemyTypes.Num() - 1);
	APawn* NewEnemy = nullptr;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0, 0, (SpaceBetweenPlatforms / 2) + (SpaceBetweenPlatforms * FMath::RandRange(1, PlatformCount-1))));
	if (EnemyTypes.IsValidIndex(SelectedEnemyType)) {
		NewEnemy = GetWorld()->SpawnActor<APawn>(EnemyTypes[SelectedEnemyType], SpawnTransform, SpawnParameters);
		//GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, FString::Printf(TEXT("Spawning... %s"), *NewEnemy->GetName()));
	} else {
		//NewEnemy = GetWorld()->SpawnActor<APawn>(AEnemyPawn_Basic_C::StaticClass(), SpawnTransform, SpawnParameters);
	}
	if (!NewEnemy) return;

	/*/ Uncomment if you want new enemies to flip their move direction from their ancestors
	if (SpawnedEnemies.Num() > 0 && IsValid(SpawnedEnemies[SpawnedEnemies.Num() - 1])) {
		if (NewEnemy->GetClass()->IsChildOf<AEnemyPawn_Basic_C>() && (SpawnedEnemies[SpawnedEnemies.Num() - 1])->GetClass()->IsChildOf<AEnemyPawn_Basic_C>()) {
			((AEnemyPawn_Basic_C*)NewEnemy)->SetFlyLeft(!((AEnemyPawn_Basic_C*)SpawnedEnemies[SpawnedEnemies.Num() - 1])->GetFlyLeft());
		}
	}
	//*/

	SpawnedEnemies.Add(NewEnemy);
}
