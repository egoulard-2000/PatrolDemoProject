// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoAiCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DemoAiController.h"

// Console Vars
static TAutoConsoleVariable<int32> CVarSightView(
	TEXT("AI.Debug.Sight.ShowVisionCone"),
	0,
	TEXT("Toggles Sight Vision Cone Drawings on NPCs <0/1>\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

ADemoAiCharacter::ADemoAiCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADemoAiCharacter::BeginPlay()
{
	Super::BeginPlay();

	DemoAiController = CastChecked<ADemoAiController>(GetController(), ECastCheckedType::NullAllowed);
	const UCharacterMovementComponent* const CharacterMovementComponent = GetCharacterMovement();
	if (CharacterMovementComponent)
	{
		CachedMovementSpeed = CharacterMovementComponent->GetMaxSpeed();
	}
}

// Called every frame
void ADemoAiCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Handle Patrol Movement Speed
	UCharacterMovementComponent* const CharacterMovementComponent = GetCharacterMovement();
	const bool bPatrolSpeedEnabled = CharacterMovementComponent && CharacterMovementComponent->GetMaxSpeed() == CachedMovementSpeed * PatrolWalkSpeedMultiplier;
	if (IsInPatrol() && !bPatrolSpeedEnabled)
	{
		CharacterMovementComponent->MaxWalkSpeed *= PatrolWalkSpeedMultiplier;
	}
	else if (!IsInPatrol() && bPatrolSpeedEnabled)
	{
		CharacterMovementComponent->MaxWalkSpeed = CachedMovementSpeed;
	}

#if WITH_EDITOR && ENABLE_DRAW_DEBUG
	if (CVarSightView.GetValueOnGameThread() != 0)
	{
		const UWorld* const World = GetWorld();
		if (!World) 
		{ 
			return;
		}

		FVector EyesLocation; FRotator UNUSED_Rotation;
		GetActorEyesViewPoint(EyesLocation, UNUSED_Rotation);

		const float HalfAngleRadians = FMath::DegreesToRadians(ConeHalfAngle);
		constexpr uint8 NumSides = 16;
		constexpr float LineThickness = 10.f;
		DrawDebugCone(World, EyesLocation, GetActorForwardVector(), MaxSightThreshold, HalfAngleRadians, HalfAngleRadians, NumSides, FColor::Magenta, false, -1.f, 0, LineThickness);
	}
#endif // WITH_EDITOR && ENABLE_DRAW_DEBUG
}
