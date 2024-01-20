// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class IMAGINATIONPROJECT_API ADoor : public AActor
{
	GENERATED_BODY()

	bool updateLocation;
	float time = 0.0f;

public:	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		bool isOpen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float openAndCloseDuraiton = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		FVector openLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		FVector closeLocation;

	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void OpenDoor();
	virtual void CloseDoor();
};
