// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UpgradeZone.h"
#include "UpgradeZone1Scale.generated.h"

/**
 * 
 */
UCLASS()
class IMAGINATIONPROJECT_API AUpgradeZone1Scale : public AUpgradeZone
{
	GENERATED_BODY()
	
		FVector startScale;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float pawnTargeScaleZ = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variable")
		float checkGroundDistance = 110;

protected:
	void SpecificUpgrade(float time, float duration)override;
	void Upgrade(AMyPawn* pawn)override;

public:
	bool IsAvailable(AMyPawn* pawn)override;

};
