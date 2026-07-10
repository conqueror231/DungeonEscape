// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Spheres.h"
#include "BaseUnit.h"
#include "ACharacter.generated.h"


class UInputMappingContext;
class UInputAction;

UCLASS()
class DUNGEONESCAPE_API AACharacter : public ABaseUnit
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AACharacter();

protected:



	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "SpheresParticles")
	FLinearColor QuasColor;
	
	UPROPERTY(EditAnywhere, Category = "SpheresParticles")
	FLinearColor WexColor;

	UPROPERTY(EditAnywhere, Category = "SpheresParticles")
	FLinearColor ExortColor;


	UPROPERTY(EditAnywhere, Category = "Skills")
	TArray<ESphereType> currentSpheres = { ESphereType::Quas, ESphereType::Quas, ESphereType::Quas};

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MinPitch = -20.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float MaxPitch = 20.0f;

	float CurrentPitch = 0.0f;
	UPROPERTY()
	class USpringArmComponent* SpringArm;
	UPROPERTY(EditDefaultsOnly,  Category= "Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* JumpAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* FirstSphereAction;

	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputAction* SecondSphereAction;

	UPROPERTY(EditDefaultsOnly,  Category = "Input")
	UInputAction* ThirdSphereAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* LookAction;
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	class UStaticMeshComponent* Body2;

	UPROPERTY(EditDefaultsOnly, Category = "MaterialsForSpheres")
	class UMaterialInterface* QuasMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "MaterialsForSpheres")
	class UMaterialInterface* WexMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "MaterialsForSpheres")
	class UMaterialInterface* ExortMaterial;

	UPROPERTY()
	UChildActorComponent* SpheresHandlerComponent;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	void HandleJump(const FInputActionValue& Value);

	void HandleStopJump(const FInputActionValue& Value);

	void FirstSphere(const FInputActionValue& Value);
	void SecondSphere(const FInputActionValue& Value);
	void ThirdSphere(const FInputActionValue& Value);

private:
	void ChangeSpheres(ESphereType sphereType);
	class UMaterialInterface* GetMaterialBySphereType(ESphereType sphereType);
    FLinearColor GetColorBySphereType(ESphereType sphereType);
public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
