// Fill out your copyright notice in the Description page of Project Settings.


#include "PhysicalActor.h"

// Sets default values
APhysicalActor::APhysicalActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
    this->SetRootComponent(mesh);

    if (mesh)
    {
        SetRootComponent(mesh);
    }

    PhysicsConstraint = CreateDefaultSubobject<UPhysicsConstraintComponent>(TEXT("PhysicsConstraintComponent"));
    if (PhysicsConstraint)
    {
        PhysicsConstraint->SetupAttachment(RootComponent);
        FConstraintInstance ConstraintInstance = PhysicsConstraint->ConstraintInstance;

        ConstraintInstance.SetLinearXMotion(ELinearConstraintMotion::LCM_Free);
        ConstraintInstance.SetLinearYMotion(ELinearConstraintMotion::LCM_Free);
        ConstraintInstance.SetLinearZMotion(ELinearConstraintMotion::LCM_Free);

        PhysicsConstraint->ConstraintInstance = ConstraintInstance;;
        PhysicsConstraint->ConstraintActor1 = this;
    }

    shpereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
    if (shpereComponent)
    {
        shpereComponent->SetupAttachment(RootComponent);
        shpereComponent->SetNotifyRigidBodyCollision(true);
        shpereComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        shpereComponent->SetSimulatePhysics(true);
        
        shpereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
        shpereComponent->SetUseCCD(true);
        shpereComponent->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
        shpereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
        shpereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
        FScriptDelegate Delegate;
        Delegate.BindUFunction(this, FName("OnHit"));

        shpereComponent->OnComponentHit.Add(Delegate);
    }

    cableComponent = CreateDefaultSubobject<UCableComponent>(TEXT("CableComponent"));
    cableComponent->SetupAttachment(PhysicsConstraint);

    cableComponent->NumSegments = 1;
}

void APhysicalActor::OnHit(UPrimitiveComponent* hitcomponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    UE_LOG(LogTemp, Warning, TEXT("CallThisMETHOD"));
}

// Called when the game starts or when spawned
void APhysicalActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APhysicalActor::Tick(float DeltaTime)  
{
	Super::Tick(DeltaTime);

}

