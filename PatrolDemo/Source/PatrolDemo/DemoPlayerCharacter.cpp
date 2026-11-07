// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

ADemoPlayerCharacter::ADemoPlayerCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Camera Setup
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Character Camera"));
	CameraComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ADemoPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADemoPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADemoPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();

	const float HorizontalMoveValue = InputValue.X;
	if (HorizontalMoveValue)
	{
		AddMovementInput(GetActorRightVector(), HorizontalMoveValue);
	}

	const float VerticalMoveValue = InputValue.Y;
	if (VerticalMoveValue)
	{
		AddMovementInput(GetActorForwardVector(), VerticalMoveValue);
	}
}

void ADemoPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D InputValue = Value.Get<FVector2D>();

	if (InputValue.X)
	{
		AddControllerYawInput(InputValue.X * HorizontalMouseSensitivity);
	}

	if (InputValue.Y)
	{
		AddControllerPitchInput(InputValue.Y * VerticalMouseSensitivity);
	}
}

// Called to bind functionality to input
void ADemoPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* EnhancedPlayerInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			EnhancedPlayerInputSubsystem->ClearAllMappings();
			EnhancedPlayerInputSubsystem->AddMappingContext(DefaultMapping, 0);
		}
	}

	if (UEnhancedInputComponent* const EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADemoPlayerCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADemoPlayerCharacter::Look);
	}
}
