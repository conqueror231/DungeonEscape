#include "ACharacter.h"
#include "Components/ChildActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInterface.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "NiagaraComponent.h"


AACharacter::AACharacter()
{
    PrimaryActorTick.bCanEverTick = true;

    Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
    Body->SetupAttachment(RootComponent);

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);




}

void AACharacter::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}

void AACharacter::BeginPlay()
{
    Super::BeginPlay();
    SpheresHandlerComponent = FindComponentByClass<UChildActorComponent>();

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}

void AACharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {
        EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AACharacter::Move);
        EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AACharacter::Look);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AACharacter::HandleJump);
        EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AACharacter::HandleStopJump);
        EnhancedInput->BindAction(FirstSphereAction, ETriggerEvent::Completed, this, &AACharacter::FirstSphere);
        EnhancedInput->BindAction(SecondSphereAction, ETriggerEvent::Completed, this, &AACharacter::SecondSphere);
        EnhancedInput->BindAction(ThirdSphereAction, ETriggerEvent::Completed, this, &AACharacter::ThirdSphere);
    }
}

void AACharacter::Move(const FInputActionValue& Value)
{
    const FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}
void AACharacter::FirstSphere(const FInputActionValue& Value) {
    ChangeSpheres(ESphereType::Quas);
}

void AACharacter::SecondSphere(const FInputActionValue& Value) {
    ChangeSpheres(ESphereType::Wex);
}

void AACharacter::ThirdSphere(const FInputActionValue& Value) {
    ChangeSpheres(ESphereType::Exort);
}

void AACharacter::ChangeSpheres(ESphereType sphereType) {
    currentSpheres[2] = currentSpheres[1];
    currentSpheres[1] = currentSpheres[0];
    currentSpheres[0] = sphereType;

    auto SphereToString = [](ESphereType Sphere) -> FString
        {
            switch (Sphere)
            {
            case ESphereType::Quas:  return TEXT("Quas");
            case ESphereType::Wex:   return TEXT("Wex");
            case ESphereType::Exort: return TEXT("Exort");
            default:                 return TEXT("Unknown");
            }
        };

    UE_LOG(LogTemp, Warning, TEXT("Current Spheres: [%s, %s, %s]"),
        *SphereToString(currentSpheres[0]),
        *SphereToString(currentSpheres[1]),
        *SphereToString(currentSpheres[2]));

    if (!SpheresHandlerComponent)
        return;
    AActor* SpheresActor = SpheresHandlerComponent->GetChildActor();

    if (!SpheresActor)
        return;
        
    TArray<UActorComponent*> FoundComponents;
    SpheresActor->GetComponents(FoundComponents);

    for (UActorComponent* Comp : FoundComponents)
            {
                UChildActorComponent* ChildComp = Cast<UChildActorComponent>(Comp);
                if (!ChildComp)
                    continue;
                AActor* SphereActor = ChildComp->GetChildActor();
                UStaticMeshComponent* MeshComp = SphereActor->FindComponentByClass<UStaticMeshComponent>();
                if (!MeshComp)
                    continue;

                if (!MeshComp)
                    return;

                UNiagaraComponent* NiagaraComp = SphereActor->FindComponentByClass<UNiagaraComponent>();
                if (!NiagaraComp)
                    return;

                UE_LOG(LogTemp, Warning, TEXT("MeshComp est"));
                if (Comp->ComponentHasTag(FName("Sphere1")))
                {
                    MeshComp->SetMaterial(0, GetMaterialBySphereType(currentSpheres[0]));

                    NiagaraComp->SetVariableLinearColor(FName("SphereColor"), GetColorBySphereType(currentSpheres[0]));
                    NiagaraComp->Activate(true);

                }
                if (Comp->ComponentHasTag(FName("Sphere2")))
                {
                    MeshComp->SetMaterial(0, GetMaterialBySphereType(currentSpheres[1]));

                    NiagaraComp->SetVariableLinearColor(FName("SphereColor"), GetColorBySphereType(currentSpheres[1]));
                    NiagaraComp->Activate(true);
                  
                }
                if (Comp->ComponentHasTag(FName("Sphere3")))
                {
                    MeshComp->SetMaterial(0, GetMaterialBySphereType(currentSpheres[2]));
                    NiagaraComp->SetVariableLinearColor(FName("SphereColor"), GetColorBySphereType(currentSpheres[2]));
                    NiagaraComp->Activate(true);
                 
                }
                
     
				

               

           
        

    }
}
UMaterialInterface* AACharacter::GetMaterialBySphereType(ESphereType sphereType) {
    if (sphereType == ESphereType::Quas)
        return QuasMaterial;
    if (sphereType == ESphereType::Wex)
        return WexMaterial;
    if (sphereType == ESphereType::Exort)
        return ExortMaterial;
    return QuasMaterial;



}

FLinearColor AACharacter::GetColorBySphereType(ESphereType sphereType)
{
    if (sphereType == ESphereType::Quas)
        return QuasColor;
    if (sphereType == ESphereType::Wex)
        return WexColor;
    if (sphereType == ESphereType::Exort)
        return ExortColor;
    return QuasColor;
}

void AACharacter::HandleJump(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("Jump() called"));

    if (GetCharacterMovement() && GetCharacterMovement()->IsMovingOnGround())
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is on ground, jumping"));
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Character is NOT on ground (falling or in air)"));
    }

    Super::Jump();
}

void AACharacter::HandleStopJump(const FInputActionValue& Value)
{
    UE_LOG(LogTemp, Warning, TEXT("StopJump() called"));
    StopJumping();
}

void AACharacter::Look(const FInputActionValue& Value)
{
    const FVector2D dir = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        FRotator NewRotation = Controller->GetControlRotation();
        NewRotation.Yaw += dir.X;

        Controller->SetControlRotation(NewRotation);
    }
    if (SpringArm != nullptr)
    {
        CurrentPitch += dir.Y;
        CurrentPitch = FMath::Clamp(CurrentPitch, MinPitch, MaxPitch);

        FRotator SpringArmRotation = SpringArm->GetRelativeRotation();
        SpringArmRotation.Pitch = CurrentPitch;
        SpringArm->SetRelativeRotation(SpringArmRotation);
    }
}