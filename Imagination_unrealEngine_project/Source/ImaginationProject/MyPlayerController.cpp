// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayerController.h"
#include "MyPawn.h"

void AMyPlayerController::TurnCamera()
{
}

void AMyPlayerController::Move(FVector2D value)
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->AddInputMovement(FVector(value.X, value.Y,0.0f));
}

void AMyPlayerController::Rotate(float value)
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->AddYawInputRotation(value);
}

void AMyPlayerController::RotateCamera(FVector2D value)
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->RotateCamera(value);
}

void AMyPlayerController::Jump()
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->Jump();
}

void AMyPlayerController::EnableGrapnel ()
{
	UE_LOG(LogTemp, Log, TEXT("EnableGrapnel "));
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->FindPointAndGrap();
}

void AMyPlayerController::DisableGrapnel ()
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->DisableGrapnel ();
}

void AMyPlayerController::TakeOrPlaceItem()
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->TakeOrPlaceItem();
}


void AMyPlayerController::UpgradePawn()
{
	AMyPawn* pawn = static_cast<AMyPawn*>(GetPawn());
	pawn->UpgradePawn();
}


AMyPlayerController::AMyPlayerController()
{
	
}


