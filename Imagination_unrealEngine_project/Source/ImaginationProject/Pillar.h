// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Pillar.generated.h"

UCLASS()
class IMAGINATIONPROJECT_API APillar : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APillar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		int pilarLineCount = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		int pilarColumnCount = 50;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float spaceBetweenLine = 2000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float spaceBetweenColumn = 2000.0f;

	TArray<UStaticMeshComponent*> CubesMesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FVector ConvertToNearestTilePosition(FVector inputPosition);

	
};
