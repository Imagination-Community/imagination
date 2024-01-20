// Fill out your copyright notice in the Description page of Project Settings.


#include "Pillar.h"
#include "Kismet/GameplayStatics.h"
#include "MyPawn.h"
// Sets default values
APillar::APillar()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    PrimaryActorTick.bCanEverTick = true;
    bAsyncPhysicsTickEnabled = true;
    USceneComponent* baseSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    if (baseSceneComponent)
        SetRootComponent(baseSceneComponent);
    
    ConstructorHelpers::FObjectFinder<UStaticMesh> CubeMeshAsset(TEXT("StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    ConstructorHelpers::FObjectFinder<UMaterial> MaterialAsset(TEXT("Material'/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial'"));
    
    float x = -spaceBetweenColumn * (pilarColumnCount / 2);
    float y = -spaceBetweenLine * (pilarLineCount / 2);

    FVector location = FVector(x, y, 5000);

    for (int i = 0; i < pilarColumnCount; i++)
    {
        for (int j = 0; j < pilarLineCount; j++)
        {       
            FString CubeName = FString(TEXT("Cube Mesh")) + FString::FromInt(i * pilarLineCount + j);

            UStaticMeshComponent* CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(*CubeName);

            if (CubeMesh)
            {
                CubeMesh->SetupAttachment(baseSceneComponent);
                CubeMesh->SetStaticMesh(CubeMeshAsset.Object);
                CubeMesh->SetMaterial(0, MaterialAsset.Object);
                CubeMesh->SetWorldLocation(location);
                CubeMesh->SetRelativeScale3D(FVector(1, 1, 100));
                CubesMesh.Add(CubeMesh);

                // Desactive Pilar In Center of World
                if (location.X == 0 && location.Y == 0)
                {
                    CubeMesh->SetHiddenInGame(true);
                    CubeMesh->SetVisibility(false);
                    CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                }
                else
                {
                    CubeMesh->SetVisibility(true);
                    CubeMesh->SetHiddenInGame(false);
                    CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                }
            }

            
            location.Y += spaceBetweenLine;

        }
        location.Y = -spaceBetweenLine * (pilarLineCount / 2);


        location.X += spaceBetweenColumn;
    }
    
    
}

// Called when the game starts or when spawned
void APillar::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APillar::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

 
    APawn* pawn = UGameplayStatics::GetPlayerPawn(this, 0);
    if (pawn)
    {
        AMyPawn* myPawn = static_cast<AMyPawn*>(pawn);
        if (myPawn)
        {
            FVector cameraLocaiton = ConvertToNearestTilePosition(myPawn->springArmComponent->GetComponentLocation());
            float x = cameraLocaiton.X - spaceBetweenColumn * (pilarColumnCount / 2);
            float y = cameraLocaiton.Y - spaceBetweenLine * (pilarLineCount / 2);

            FVector location = FVector(x, y, 5000);

            for (int i = 0; i < pilarColumnCount; i++)
            {
                for (int j = 0; j < pilarLineCount; j++)
                {
                    UStaticMeshComponent* CubeMesh = CubesMesh[j + i * pilarLineCount];

                    if (CubeMesh)
                    {
                        CubeMesh->SetWorldLocation(location);

                        // Desactive Pilar In Center of World
                        if (location.X == 0 && location.Y == 0 || location.Y > 85975.0f)
                        {
                            if (CubeMesh->IsVisible())
                            {
                                CubeMesh->SetVisibility(false);
                                CubeMesh->SetHiddenInGame(true);
                                CubeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
                            }

                        }
                        else
                        {
                            if (!CubeMesh->IsVisible())
                            {
                                CubeMesh->SetHiddenInGame(false);
                                CubeMesh->SetVisibility(true);
                                CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
                            }

                        }
                    }


                    location.Y += spaceBetweenLine;

                }
                location.Y = cameraLocaiton.Y - spaceBetweenLine * (pilarLineCount / 2);


                location.X += spaceBetweenColumn;
            }

        }
    }
}

FVector APillar::ConvertToNearestTilePosition(FVector inputPosition)
{
    int xIndex = FMath::RoundToInt(inputPosition.X / spaceBetweenColumn);
    int yIndex = FMath::RoundToInt(inputPosition.Y / spaceBetweenLine);

    FVector nearestTilePosition = FVector(xIndex * spaceBetweenColumn, yIndex * spaceBetweenLine, 0.0f);

    return nearestTilePosition;
}