// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemTrigger.h"

// Sets default values
AItemTrigger::AItemTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemTrigger::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemTrigger::TakeItem()
{
	IsItemAvailable = false;
	OnTakeItem.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("TakeItem"));

}

void AItemTrigger::PlaceItem()
{
	IsItemAvailable = true;	
	OnPlaceItem.Broadcast();

	UE_LOG(LogTemp, Log, TEXT("PlaceItem;"));

}