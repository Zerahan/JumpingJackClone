// Fill out your copyright notice in the Description page of Project Settings.


#include "Core/World/Handler_Basic_C.h"

// Sets default values for this component's properties
UHandler_Basic_C::UHandler_Basic_C()
	:Super()
{
	PrimaryComponentTick.bCanEverTick = false;
	DoFullReset = true;
	MapWidth = 1;
	MapHeight = 1;
	PlatformCount = 1;
	SpaceBetweenPlatforms = 1;
}


// Called when the game starts
void UHandler_Basic_C::BeginPlay()
{
	Super::BeginPlay();
}

void UHandler_Basic_C::SetDifficultyLevel_Implementation(int NewDifficultyLevel)
{
}

void UHandler_Basic_C::InjectMapSize(const int& _Width, const int& _Height, const uint8& _PlatformCount) {
	MapWidth = _Width;
	MapHeight = _Height;
	PlatformCount = _PlatformCount;
	SpaceBetweenPlatforms = MapHeight / (PlatformCount);
}

void UHandler_Basic_C::Reset_Implementation(bool ForceFullReset)
{
	if (DoFullReset) {
		FullReset();
	}
}

void UHandler_Basic_C::FullReset_Implementation()
{
	DoFullReset = false;
}

void UHandler_Basic_C::Update_Implementation()
{
}

void UHandler_Basic_C::Add_Implementation()
{
}
