// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "UObject/Object.h"
#include "TeleporterTrigger.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMyPawn::AMyPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bAsyncPhysicsTickEnabled = true;
    USceneComponent* baseSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));  
    if (baseSceneComponent)
        SetRootComponent(baseSceneComponent);

    CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cube Mesh"));
    if (CubeMesh)
    {
        CubeMesh->SetupAttachment(baseSceneComponent);
        CubeMesh->SetSimulatePhysics(true);
    }
    
    idText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("IDtext"));

    if (idText)
    {
        idText->SetupAttachment(CubeMesh);
    }

/*    SpringArmTarget = CreateDefaultSubobject<USceneComponent>(TEXT("SpringArmTarget"));
    if (SpringArmTarget)
        SpringArmTarget->SetupAttachment(SpringArmTarget);
     */
    springArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    if (springArmComponent)
    {
        springArmComponent->SetupAttachment(CubeMesh);
    }
}

// Called when the game starts or when spawned
void AMyPawn::BeginPlay()
{
	Super::BeginPlay();

    FAttachmentTransformRules fAttachement = FAttachmentTransformRules(EAttachmentRule::KeepRelative, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false);

    FVector axeReference = FVector(1, 0, 0);
    FVector forward = CubeMesh->GetForwardVector();
    forward.Z = 0.0f;
    forward.Normalize();
    FQuat RotationQuat = FQuat::FindBetweenNormals(axeReference, forward);

    float startRotation = 20.0f;
    springArmComponent->SetWorldRotation(RotationQuat * FQuat(FVector(0,1,0), FMath::DegreesToRadians(startRotation)));
}

// Called every frame
void AMyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    FRotator pawnRotation = GetActorRotation();

    if (IsValid(CubeMesh) && IsValid(springArmComponent))
    {                 
        FVector currentVelocity = CubeMesh->GetPhysicsLinearVelocity();

        //CheckJump
        if (m_isJump)
        {
            if (currentVelocity.Z <= 0.0f)
            {
                m_isJump = false;
                UE_LOG(LogTemp, Log, TEXT("finnishedJump"));
            }
        }

        //Check mustJump
        if (m_mustJump)
        {
            float zTargetVelocity = FMath::Sqrt((double)(m_jumpHeightRuntime * -2.0f * GetWorld()->GetGravityZ()));

            FVector Impulse = FVector(currentVelocity.X, currentVelocity.Y, zTargetVelocity);

            if(!isInUpgradeState)
            CubeMesh->AddImpulse(Impulse, NAME_None, true);

            m_isJump = true;
            m_mustJump = false;
        }

        if (!isInUpgradeState)
        {
            if (!m_isGrapnel)
            {
                if (IsOnGround())
                {
                    // Apply horizontalforce relative to camera direction
                    FVector forwardMovement = springArmComponent->GetForwardVector() * m_inputMovement.X;
                    FVector rightMovement = springArmComponent->GetRightVector() * m_inputMovement.Y;
                    FVector forceToApply = (rightMovement + forwardMovement) * MoveForceMultiplier;

                    if (!m_isGrapnel)
                        CubeMesh->AddForce(forceToApply, NAME_None, true);

                    // Apply horizontalBrakingForce
                    FVector horizontalVelocity = FVector(currentVelocity.X, currentVelocity.Y, 0);
                    FVector horizontalBrakingForce = -breakingforce * horizontalVelocity.GetSafeNormal();
                    CubeMesh->AddForce(horizontalBrakingForce, NAME_None, true);

                    // Clamp velocity
                    FVector clampedHorizontalVelocity = horizontalVelocity.GetClampedToMaxSize(MaxSpeed);
                    clampedHorizontalVelocity.Z = currentVelocity.Z;
                    CubeMesh->SetPhysicsLinearVelocity(clampedHorizontalVelocity, false, NAME_None);

                    //Apply torque
                    if (!forceToApply.IsNearlyZero())
                    {
                        forceToApply.Z = 0.0f;
                        FVector targetDirection = forceToApply.GetSafeNormal();
                        FVector currentDirection = CubeMesh->GetForwardVector();

                        FVector rotationAxis = FVector::CrossProduct(currentDirection, targetDirection);
                        float rotationAngle = FMath::Acos(FVector::DotProduct(currentDirection, targetDirection));

                        float torqueFactor = 1000.0f;

                        FVector torqueToApply = rotationAxis.GetSafeNormal() * rotationAngle * torqueFactor;

                        CubeMesh->AddTorqueInRadians(torqueToApply, NAME_None, true);
                    }


                    // Apply yaw damping
                    FVector angularVelocity = CubeMesh->GetPhysicsAngularVelocityInDegrees();
                    FVector angularDampingTorque = FVector(0, 0, -angularYawDamping * angularVelocity.Z);
                    CubeMesh->AddTorqueInRadians(angularDampingTorque, NAME_None, true);

                    //Clamp Yaw Angular velocty
                    angularVelocity = CubeMesh->GetPhysicsAngularVelocityInDegrees();
                    angularVelocity.Z = FMath::Clamp(FMath::Abs(angularVelocity.Z), 0, maxAngularYawVelocity) * FMath::Sign(angularVelocity.Z);
                    CubeMesh->SetPhysicsAngularVelocityInDegrees(angularVelocity, false, NAME_None);
                }
            }
            else
            {               
                CubeMesh->SetWorldLocation(physicalActor->shpereComponent->GetComponentLocation());
            }
        }      
    }

    FVector location = CubeMesh->GetComponentLocation();

    //auto teleport North-South
    if (location.X > maxXLocation)
    {
        location.X = minXLocation;
        CubeMesh->SetWorldLocation(location, false, nullptr, ETeleportType::TeleportPhysics);
    }
    else if (location.X < minXLocation)
    {
        location.X = maxXLocation;
        CubeMesh->SetWorldLocation(location, false, nullptr, ETeleportType::TeleportPhysics);
    }

    // Reset movement input
    m_inputMovement = FVector::Zero();
    m_inputRotation = FVector::Zero();
}

bool AMyPawn::IsOnGround()
{
    FVector groundPoint1, groundPoint2, groundPoint3, groundPoint4;
    FVector characterLocation = CubeMesh->GetComponentLocation();
    FVector startPoint1 = characterLocation + FVector(-1.0f, -1.0f, 0).GetSafeNormal() * checkGroundDistanceHorizontalOffset;
    FVector startPoint2 = characterLocation + FVector(1.0f, -1.0f, 0).GetSafeNormal() * checkGroundDistanceHorizontalOffset;
    FVector startPoint3 = characterLocation + FVector(-1.0f, 1.0f, 0).GetSafeNormal() * checkGroundDistanceHorizontalOffset;
    FVector startPoint4 = characterLocation + FVector(1.0f, 1.0f, 0).GetSafeNormal() * checkGroundDistanceHorizontalOffset;

    bool findGround1 = ComputeGroundPoint(startPoint1, groundPoint1);
    bool findGround2 = ComputeGroundPoint(startPoint2, groundPoint2);
    bool findGround3 = ComputeGroundPoint(startPoint3, groundPoint3);
    bool findGround4 = ComputeGroundPoint(startPoint4, groundPoint4);

    if (!findGround1 && !findGround2 && !findGround3 && !findGround4)
        return false;

    if (findGround1 && (startPoint1 - groundPoint1).Length() < checkGroundDistance)
        return true;

    if (findGround2 && (startPoint2 - groundPoint2).Length() < checkGroundDistance)
        return true;

    if (findGround3 && (startPoint3 - groundPoint3).Length() < checkGroundDistance)
        return true;

    if (findGround4 && (startPoint4 - groundPoint4).Length() < checkGroundDistance)
        return true;

    return false;
}

// Called fiexed frame
void AMyPawn::AsyncPhysicsTickActor(float DeltaTime, float SimTime)
{
   
}

void AMyPawn::FindPointAndGrap()
{
    if (!haveGrapnel)
        return;

    if (m_isGrapnel)
        return;

    FHitResult HitResult;
    FVector Start = CubeMesh->GetComponentLocation();
    FVector groundHitPoint = FVector(0,0,0);
    FVector ceilHitPoint = FVector(0, 0, 0);

    if (ComputeGroundAndCeilPoint(Start, groundHitPoint, ceilHitPoint))
    {

        FVector point = FVector::Zero();

        if (ComputeGrapPoint(point, groundHitPoint, ceilHitPoint))
        {
            Grap(point, (point - Start).Length());
            UE_LOG(LogTemp, Log, TEXT("Grap"));
        }

    }
}


bool AMyPawn::ComputeGroundPoint(FVector point, FVector& groundPoint)
{
    FVector GroundEnd = point - FVector(0, 0, checkFloorAndCeilDistance);

    FVector groundHitPoint = FVector(0, 0, 0);
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    FHitResult HitResult;
    bool bGroundHit = UKismetSystemLibrary::LineTraceSingle(
        GetWorld(),
        point,
        GroundEnd,
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::None,
        HitResult,
        true);

    if (bGroundHit && HitResult.GetActor()->Tags.Contains("Ground"))
        groundHitPoint = HitResult.ImpactPoint;
    else
        return false;

   
    groundPoint = groundHitPoint;
    return true;
}

bool AMyPawn::ComputeGroundAndCeilPoint(FVector point,FVector& groundPoint, FVector& ceilPoint)
{
    FVector GroundEnd = point - FVector(0, 0, checkFloorAndCeilDistance);
    FVector CeilEnd = point + FVector(0, 0, checkFloorAndCeilDistance);
    FVector groundHitPoint = FVector(0, 0, 0);
    FVector ceilHitPoint = FVector(0, 0, 0);
    FHitResult HitResult;
    TArray<AActor*> ActorsToIgnore;
    ActorsToIgnore.Add(this);
    bool bGroundHit = UKismetSystemLibrary::LineTraceSingle(
        GetWorld(),
        point,
        GroundEnd,
        ETraceTypeQuery::TraceTypeQuery1,
        false,
        ActorsToIgnore, 
        EDrawDebugTrace::None,
        HitResult,
        true);

    if (bGroundHit && HitResult.GetActor()->Tags.Contains("Ground"))
        groundHitPoint = HitResult.ImpactPoint;
    else
        groundHitPoint = GroundEnd;


    UE_LOG(LogTemp, Log, TEXT("GroundCheked"));

    bool bCeilHit = UKismetSystemLibrary::LineTraceSingle(
        GetWorld(),
        point,
        CeilEnd,
        ETraceTypeQuery::TraceTypeQuery1, 
        false,
        ActorsToIgnore, 
        EDrawDebugTrace::None,
        HitResult,
        true);

    if (bCeilHit && HitResult.GetActor()->Tags.Contains("Ceil"))
        ceilHitPoint = HitResult.ImpactPoint;
    else
        return false;

    groundPoint = groundHitPoint;
    ceilPoint = ceilHitPoint;

    return true;
}

//To Do : get any item (here the grapnel item is in hard code)
void AMyPawn::TakeOrPlaceItem()
{
    if (m_itemTrigger)
    {
        if (m_itemTrigger->IsGetterTrigger && m_itemTrigger->IsItemAvailable)
        {

            m_itemTrigger->TakeItem();
            AddItem();
        }
        else if (!m_itemTrigger->IsGetterTrigger && !m_itemTrigger->IsItemAvailable)
        {
            m_itemTrigger->PlaceItem();
            RemoveItem();
        }
    }
}

void AMyPawn::AddItem()
{
    UE_LOG(LogTemp, Log, TEXT("AddItem %"));
    haveGrapnel = true;
    OnAddItem.Broadcast();
}

void AMyPawn::RemoveItem()
{
    UE_LOG(LogTemp, Log, TEXT("RemoveItem %"));
    haveGrapnel = false;
    OnRemoveItem.Broadcast();
}



void AMyPawn::Jump()
{
    if (m_isGrapnel)
        return;
    
    if (!m_mustJump && !m_isJump && IsOnGround())
    {
        m_mustJump = true;
        m_jumpHeightRuntime = jumpHeight;
    }
   
}

bool AMyPawn::ComputeGrapPoint(FVector& point, FVector groundPoint, FVector ceilPoint)
{
    FVector Start = CubeMesh->GetComponentLocation();

    float groundDistance = (groundPoint - Start).Length();
    float currentDistanceFromCeil = (ceilPoint - Start).Length(); 

    if (currentDistanceFromCeil <= grabMaxDistance)
    {
        if (groundDistance > grabMinimalDistanceFromGround)
        {
            float grapLength = (ceilPoint - groundPoint).Length() - grabMinimalDistanceFromGround;
            grapLength = FMath::Min(grapLength, grabMaxDistance);
            UE_LOG(LogTemp, Log, TEXT("Compute GrapLength %f"), grapLength);

            float grapHorizontalDistance = FMath::Sqrt(grapLength * grapLength - currentDistanceFromCeil * currentDistanceFromCeil);
            TArray<AActor*> ActorsToIgnore;
            ActorsToIgnore.Add(this);
            FHitResult HitResult;
            FVector forward = springArmComponent->GetForwardVector();
            forward.Z = 0.0f;
            forward.Normalize();
            FVector target = Start + forward * grapHorizontalDistance + FVector(0, 0, currentDistanceFromCeil);
            FVector targetDirecton = (forward * grapHorizontalDistance + FVector(0, 0, currentDistanceFromCeil)).GetSafeNormal();

            bool bHit = UKismetSystemLibrary::LineTraceSingle(
                GetWorld(),
                Start,
                Start + targetDirecton * (grabMaxDistance + 1.0f),
                ETraceTypeQuery::TraceTypeQuery1,
                false,
                ActorsToIgnore,
                EDrawDebugTrace::None,
                HitResult,
                true);

            if (bHit && HitResult.GetActor()->Tags.Contains("Ceil"))
            {
                point = HitResult.ImpactPoint;

                float SphereRadius = 10.0f;
                float Duration = 50.0f;
                int32 Segments = 12;
                FColor SphereColor = FColor::Red;

                DrawDebugSphere(
                    GetWorld(),
                    point,
                    SphereRadius,
                    Segments,
                    SphereColor,
                    false,
                    Duration,
                    (uint8)'\000',
                    1.0f
                );

                return true;
            }
        }

       

    }

    return false;
}

void AMyPawn::OnHit(UPrimitiveComponent* hitcomponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
    if (hitcomponent == CubeMesh && OtherComp->ComponentTags.Contains("DeadZone"))
    {
        CubeMesh->SetWorldLocation(FVector(0,0,60), false, nullptr, ETeleportType::ResetPhysics);
        CubeMesh->SetWorldRotation(FQuat::Identity, false, nullptr, ETeleportType::ResetPhysics);
        m_isJump = false;
        m_mustJump = false;
    }
}

void AMyPawn::OnComponentBeginOverlap(UPrimitiveComponent* overlapComponent, UPrimitiveComponent* OtherComp)
{
    if (overlapComponent == CubeMesh)
    {
        if (OtherComp->ComponentTags.Contains("ItemTrigger"))
        {

            AActor* actor = OtherComp->GetAttachParentActor();
            if (actor)
            {
                AItemTrigger* itemTrigger = static_cast<AItemTrigger*>(actor);
                if (itemTrigger)
                {
                    UE_LOG(LogTemp, Log, TEXT("EnterTrigerItem"));
                    m_itemTrigger = itemTrigger;
                }
            }
        }
        else if (OtherComp->ComponentTags.Contains("UpgradeZoneTrigger"))
        {
            AActor* actor = OtherComp->GetAttachParentActor();
            if (actor)
            {
                AUpgradeZone* upgradeZone = static_cast<AUpgradeZone*>(actor);
                if (upgradeZone)
                {
                    UE_LOG(LogTemp, Log, TEXT("EnterUpgradeZone %"));
                    m_upgradeZone = upgradeZone;
                }
            }
        }

    }
}

void AMyPawn::OnComponentEndOverlap(UPrimitiveComponent* overlapComponent, UPrimitiveComponent* OtherComp)
{
    if (overlapComponent == CubeMesh)
    {
        if (OtherComp->ComponentTags.Contains("ItemTrigger"))
        {
            AActor* actor = OtherComp->GetAttachParentActor();
            if (actor)
            {
                AItemTrigger* itemTrigger = static_cast<AItemTrigger*>(actor);
                if (itemTrigger)
                {
                    UE_LOG(LogTemp, Log, TEXT("ExitTrigerItem %"));
                    m_itemTrigger = nullptr;
                }
            }
        }
        else if (OtherComp->ComponentTags.Contains("TeleporterTrigger"))
        {
            AActor* actor = OtherComp->GetAttachParentActor();
            if (actor)
            {
                ATeleporterTrigger* teleporterTrigger = static_cast<ATeleporterTrigger*>(actor);
                if (teleporterTrigger)
                {
                    switch (teleporterTrigger->TeleporterType)
                    {
                    case ETeleporterType::Both:
                        if (teleporterTrigger->connectedTeleportTrigger)
                        {
                            
                            FVector relativeLocation = CubeMesh->GetComponentLocation() - OtherComp->GetComponentLocation();

                           
                            if (FVector::DotProduct(relativeLocation.GetSafeNormal(), teleporterTrigger->teleporterDirection) > 0)
                            {
                                FVector teleportLocation = teleporterTrigger->connectedTeleportTrigger->GetActorLocation() + relativeLocation;
                                CubeMesh->SetWorldLocation(teleportLocation, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
                            }                         
                        }

                        break;
                    case ETeleporterType::In:

                        if (teleporterTrigger->connectedTeleportTrigger)
                        {
                            FVector relativeLocation = CubeMesh->GetComponentLocation() - OtherComp->GetComponentLocation();
                            if (FVector::DotProduct(relativeLocation.GetSafeNormal(), teleporterTrigger->teleporterDirection) > 0)
                            {
                                FVector teleportLocation = teleporterTrigger->connectedTeleportTrigger->GetActorLocation() + relativeLocation;
                                CubeMesh->SetWorldLocation(teleportLocation, false, (FHitResult*)nullptr, ETeleportType::TeleportPhysics);
                            }
                        }

                        break;
                    case ETeleporterType::Out:
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        else if (OtherComp->ComponentTags.Contains("UpgradeZoneTrigger"))
        {
            AActor* actor = OtherComp->GetAttachParentActor();
            if (actor)
            {
                AUpgradeZone* upgradeZone = static_cast<AUpgradeZone*>(actor);
                if (upgradeZone)    
                {
                    UE_LOG(LogTemp, Log, TEXT("ExitUpgradeZone"));
                    m_upgradeZone = nullptr;
                }
            }
        }
    }
}

void AMyPawn::Grap(FVector point, float grapLength)
{
    m_isJump = false;
    m_mustJump = false;
    m_isGrapnel = true;
    m_grapnelPoint = point;
    m_grapnelLength = grapLength;

    if (!physicalActor)
    {
        FActorSpawnParameters SpawnParams;
        //SpawnParams.Name = FName("PhysicalActor");
        physicalActor = GetWorld()->SpawnActor<APhysicalActor>(APhysicalActor::StaticClass(), m_grapnelPoint, FRotator::ZeroRotator, SpawnParams);
        physicalActor->mesh->SetStaticMesh(CubeMesh->GetStaticMesh());
        physicalActor->mesh->SetMaterial(0, CubeMesh->GetMaterial(0));
    }

    if (physicalActor)
    {
        physicalActor->SetActorLocation(m_grapnelPoint);
        physicalActor->shpereComponent->SetWorldLocation(CubeMesh->GetComponentLocation());
        physicalActor->shpereComponent->SetPhysicsLinearVelocity(CubeMesh->GetPhysicsLinearVelocity());
        physicalActor->shpereComponent->SetPhysicsAngularVelocityInDegrees(CubeMesh->GetPhysicsAngularVelocityInDegrees());

        CubeMesh->SetSimulatePhysics(false);
        FVector torsionAxeReference = FVector(1, 0, 0);
        FVector rotationAxe = springArmComponent->GetForwardVector();
        rotationAxe.Z = 0.0f;
        rotationAxe.Normalize();

        m_grapForward = rotationAxe;
        FQuat RotationQuat = FQuat::FindBetweenNormals(torsionAxeReference, rotationAxe);
        // Configurer l'offset de rotation angulaire si nécessaire

        physicalActor->PhysicsConstraint->SetWorldRotation(RotationQuat);
        FComponentReference ref;
        ref.OtherActor = this;
        physicalActor->cableComponent->SetAttachEndToComponent(CubeMesh);
        physicalActor->cableComponent->EndLocation = FVector::ZeroVector;
        physicalActor->cableComponent->CableLength = m_grapnelLength;
        physicalActor->cableComponent->Activate();

        if (physicalActor)
        {
            PhysicsConstraint = physicalActor->PhysicsConstraint;
            if (PhysicsConstraint)
            {
                PhysicsConstraint->SetConstrainedComponents(nullptr, NAME_None, physicalActor->shpereComponent, NAME_None);
            }
        }

        FConstraintInstance ConstraintInstance = physicalActor->PhysicsConstraint->ConstraintInstance;

        ConstraintInstance.SetLinearXMotion(ELinearConstraintMotion::LCM_Locked);
        ConstraintInstance.SetLinearYMotion(ELinearConstraintMotion::LCM_Locked);
        ConstraintInstance.SetLinearZMotion(ELinearConstraintMotion::LCM_Locked);

        ConstraintInstance.SetAngularSwing1Motion(EAngularConstraintMotion::ACM_Locked);
        ConstraintInstance.SetAngularSwing2Motion(EAngularConstraintMotion::ACM_Free);
        ConstraintInstance.SetAngularTwistMotion(EAngularConstraintMotion::ACM_Locked);


        physicalActor->PhysicsConstraint->ConstraintInstance = ConstraintInstance;
        physicalActor->PhysicsConstraint->InitComponentConstraint();
    }
  

}

void AMyPawn::DisableGrapnel()
{
    m_isGrapnel = false;

    if (physicalActor)
    {
        CubeMesh->SetSimulatePhysics(true);
        CubeMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

        CubeMesh->SetWorldLocation(physicalActor->shpereComponent->GetComponentLocation());
        CubeMesh->SetPhysicsLinearVelocity(physicalActor->shpereComponent->GetPhysicsLinearVelocity());
        CubeMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);

        FConstraintInstance ConstraintInstance;

        ConstraintInstance.SetLinearXMotion(ELinearConstraintMotion::LCM_Free);
        ConstraintInstance.SetLinearYMotion(ELinearConstraintMotion::LCM_Free);
        ConstraintInstance.SetLinearZMotion(ELinearConstraintMotion::LCM_Free);
        PhysicsConstraint->SetConstrainedComponents(nullptr, NAME_None, nullptr, NAME_None);
        PhysicsConstraint->BreakConstraint();

        physicalActor->PhysicsConstraint->ConstraintInstance = ConstraintInstance;
        physicalActor->PhysicsConstraint->InitComponentConstraint();
        physicalActor->cableComponent->Deactivate();

        physicalActor->Destroy();
        physicalActor = nullptr;

        FVector torsionAxeReference = FVector(1, 0, 0);

        FQuat RotationQuat = FQuat::FindBetweenNormals(torsionAxeReference, m_grapForward);   
        CubeMesh->SetWorldRotation(RotationQuat, false, nullptr, ETeleportType::ResetPhysics);
        CubeMesh->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector);
    }

}

void AMyPawn::RotateCamera(FVector2D value)
{
    if (IsValid(springArmComponent))
    { 
        FRotator rotator = springArmComponent->GetComponentRotation();
        float newYaw = rotator.Yaw + value.X;
        float newPitch = FMath::Clamp(rotator.Pitch + value.Y, -80.f, 80.f);
        rotator = FRotator(newPitch, newYaw, rotator.Roll);
        springArmComponent->SetWorldRotation(rotator);
    }
}

// Called to bind functionality to input
void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AMyPawn::AddInputMovement(FVector value)
{
	m_inputMovement += value;
}

void  AMyPawn::AddYawInputRotation(float value)
{
    m_inputRotation.Z += value;
}

void AMyPawn::UpgradePawn()
{
    if (m_upgradeZone)
    {
        if (m_upgradeZone->IsAvailable(this))
        {
            m_upgradeZone->Upgrade(this);
            isInUpgradeState = true;
        }
    }
}
