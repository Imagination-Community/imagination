// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeZone.h"
#include "MyPawn.h"

// Sets default values
AUpgradeZone::AUpgradeZone()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AUpgradeZone::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUpgradeZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (isUpgradeStarted)
	{
		if (state == 0)
		{
			m_time += DeltaTime;
			FVector newLocation = FMath::Lerp(startLocation, targetLocation, FMath::Clamp(m_time / interpolatioDuration,0,1));
			upgradePawn->CubeMesh->SetWorldLocation(newLocation);
					
			FQuat newRotation = FQuat::Slerp(startRotation.Quaternion(), FQuat::MakeFromEuler(targetRotation), FMath::Clamp(m_time / interpolatioDuration, 0, 1));
			upgradePawn->CubeMesh->SetWorldRotation(newRotation);
			if (m_time > interpolatioDuration)
			{
				state = 1;
				m_time = 0.0f;
			}			
		}
		else if (state == 1)
		{			
			m_time += DeltaTime;
			SpecificUpgrade(m_time, upgradeDuration);

			if (m_time > upgradeDuration)
			{
				state = 2;
				m_time = 0.0f;
				isUpgradeStarted = false;
				isUpgradeCompleted = true;
				upgradePawn->isInUpgradeState = false;
				upgradePawn->OnUpgraded.Broadcast(this);
				if(upgradePawn->OnUpgraded.IsBound())
					UE_LOG(LogTemp, Log, TEXT("IsBound"));

				UE_LOG(LogTemp, Log, TEXT("PawnIsUpgraded"));

				upgradePawn->CubeMesh->SetSimulatePhysics(true);
				upgradePawn->CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
}

void AUpgradeZone::Upgrade(AMyPawn* pawn)
{
	if (!isUpgradeCompleted && !isUpgradeStarted)
	{
		isUpgradeStarted = true;
		upgradePawn = pawn;

		upgradePawn->CubeMesh->SetSimulatePhysics(false);
		upgradePawn->CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		startLocation = upgradePawn->CubeMesh->GetComponentLocation();
		startRotation = upgradePawn->CubeMesh->GetComponentRotation();
	}
}

void AUpgradeZone::SpecificUpgrade(float time, float duration)
{
	FText pawnIDText = FText::FromString(pawnID);
	upgradePawn->idText->SetText(pawnIDText);
}

bool AUpgradeZone::IsAvailable(AMyPawn* pawn)
{
	return true;
}