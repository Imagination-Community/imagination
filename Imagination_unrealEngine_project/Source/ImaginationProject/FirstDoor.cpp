// Fill out your copyright notice in the Description page of Project Settings.


#include "FirstDoor.h"
#include "MyPawn.h"
#include "Kismet/GameplayStatics.h"

void AFirstDoor::BeginPlay()
{
    ADoor::BeginPlay();
    APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (pawn)
    {
        AMyPawn* myPawn = static_cast<AMyPawn*>(pawn);
        if (myPawn)
        {
            UE_LOG(LogTemp, Log, TEXT("Add OnUpgradeEvent Event"));

            TScriptDelegate<FWeakObjectPtr> Delegate;
            Delegate.BindUFunction(this, FName("OnUpgradeEvent"));
            myPawn->OnUpgraded.Add(Delegate);
        }
    }
}

void AFirstDoor::OnUpgradeEvent(AUpgradeZone* UpgradeZone)
{
    UE_LOG(LogTemp, Log, TEXT("OnUpgradeEvent is call"));

    if (UpgradeZone->upgradeIndex == 1)
    {
        UE_LOG(LogTemp, Log, TEXT("OpenDoor"));

        OpenDoor();
    }

}