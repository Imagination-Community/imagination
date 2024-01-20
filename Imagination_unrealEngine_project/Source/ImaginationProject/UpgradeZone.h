// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UpgradeZone.generated.h"

class AMyPawn;
UCLASS()
class IMAGINATIONPROJECT_API AUpgradeZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUpgradeZone();


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		FVector targetRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		FVector targetLocation;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float interpolatioDuration = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float upgradeDuration = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		int upgradeIndex = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		FString pawnID = "8435B-0";

	bool isUpgradeCompleted;
	bool isUpgradeStarted;

	FRotator startRotation;
	FVector startLocation;

	int state = 0;
	float m_time;
	AMyPawn* upgradePawn;

	virtual void Upgrade(AMyPawn* pawn);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SpecificUpgrade(float time, float duration);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual bool IsAvailable(AMyPawn* pawn);

};
