// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacter.h"
#include "InputActionValue.h"
#include "DemoPlayerCharacter.generated.h"

class UCameraComponent;
class UInputMappingContext;
class UInputAction;

UCLASS()
class PATROLDEMO_API ADemoPlayerCharacter : public ADemoCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADemoPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	void BeginPlay() final;

	// Called every frame
	void Tick(float DeltaTime) final;

	// Called to bind functionality to input
	void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) final;

	// Handle Player Movement
	void Move(const FInputActionValue& Value);

	// Handle Player Camera Movement
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

private:
	UPROPERTY(EditAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere)
	float HorizontalMouseSensitivity = 1.f;

	UPROPERTY(EditAnywhere)
	float VerticalMouseSensitivity = 1.f;
};
