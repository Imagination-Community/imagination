// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicalConstrainTestObjetct.h"

// Sets default values
APhysicalConstrainTestObjetct::APhysicalConstrainTestObjetct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APhysicalConstrainTestObjetct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicalConstrainTestObjetct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APhysicalConstrainTestObjetct::InitComponentConstraint(UPhysicsConstraintComponent* physicalComponent)
{
	physicalComponent->InitComponentConstraint();
}