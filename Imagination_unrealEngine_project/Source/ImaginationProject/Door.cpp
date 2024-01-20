// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (updateLocation)
	{
		time += DeltaTime;
		FVector location = FVector::ZeroVector;
		if (isOpen)
			location = FMath::Lerp(closeLocation, openLocation, FMath::Clamp(time / openAndCloseDuraiton,0.0f,1.0f));
		else
			location = FMath::Lerp(openLocation, closeLocation, FMath::Clamp(time / openAndCloseDuraiton, 0.0f, 1.0f));

		SetActorLocation(location);
		if (time >= openAndCloseDuraiton)
		{
			updateLocation = false;
			time = 0.0f;
		}
	}
}

void ADoor::OpenDoor()
{
	if (!isOpen)
	{
		if(updateLocation)
			time = openAndCloseDuraiton - time;

		isOpen = true;
		updateLocation = true;
	}	
}

void ADoor::CloseDoor()
{
	if (isOpen)
	{
		isOpen = false;
		if(updateLocation)
			time = openAndCloseDuraiton - time;
		updateLocation = true;
	}
}
