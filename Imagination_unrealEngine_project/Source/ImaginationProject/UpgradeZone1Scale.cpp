// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeZone1Scale.h"
#include "MyPawn.h"

void AUpgradeZone1Scale::Upgrade(AMyPawn* pawn)
{
	if (!isUpgradeCompleted && !isUpgradeStarted)
	{
		AUpgradeZone::Upgrade(pawn);
		startScale = upgradePawn->CubeMesh->GetRelativeScale3D();
	}
}
void AUpgradeZone1Scale::SpecificUpgrade(float time, float duration)
{
	AUpgradeZone::SpecificUpgrade(time, duration);

	if (upgradePawn)
	{
		FVector newScale = FMath::Lerp(startScale, FVector(startScale.X, startScale.Y, pawnTargeScaleZ), FMath::Clamp(time / duration, 0, 1));
		upgradePawn->CubeMesh->SetRelativeScale3D(newScale);

		float relativeZScale = pawnTargeScaleZ - newScale.Z;
		upgradePawn->CubeMesh->SetWorldLocation(targetLocation + FVector(0,0, relativeZScale));
		upgradePawn->checkGroundDistance = checkGroundDistance;
	}
}

bool AUpgradeZone1Scale::IsAvailable(AMyPawn* pawn)
{
	if (!pawn->haveGrapnel)
		return true;
	else
		return false;
}
