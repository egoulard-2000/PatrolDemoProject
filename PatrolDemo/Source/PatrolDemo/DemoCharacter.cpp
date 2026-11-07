// Fill out your copyright notice in the Description page of Project Settings.

#include "DemoCharacter.h"
#include "Components/CapsuleComponent.h"

// Console Vars
static TAutoConsoleVariable<int32> CVarVisibilityDebug(
	TEXT("AI.Debug.Visibility.CanBeSeen"),
	1,
	TEXT("Toggles character sight <0/1>\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

// Sets default values
ADemoCharacter::ADemoCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADemoCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADemoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ADemoCharacter::CanBeSeen() const
{
#if WITH_EDITOR
	if (CVarVisibilityDebug.GetValueOnGameThread() == 0)
	{
		return false;
	}
#endif

	return bCanBeSeen;
}

FVector ADemoCharacter::GetCharacterFeetLocation() const
{
	const UCapsuleComponent* const CharacterCapsuleComponent = GetCapsuleComponent();
	check(CharacterCapsuleComponent);

	FVector CurrentLocation = GetActorLocation();
	const float HalfHeight = CharacterCapsuleComponent->GetScaledCapsuleHalfHeight();
	CurrentLocation.Z -= HalfHeight;

	return CurrentLocation;
}

bool ADemoCharacter::CanSeeCharacter(const ACharacter* Character) const
{
	if (!Character || Character == this)
	{
		return false;
	}

	const UWorld* const World = GetWorld();
	if (!ensure(World))
	{
		return false;
	}

	// 1. Simple can be seen check on other character
	if (const ADemoCharacter* const DemoCharacter = Cast<ADemoCharacter>(Character))
	{
		if (!DemoCharacter->CanBeSeen())
		{
			return false;
		}
	}

	// Our Eye Location
	FVector OutEyesLocation;
	FRotator UNUSED_OutEyesRotation;
	GetActorEyesViewPoint(OutEyesLocation, UNUSED_OutEyesRotation);

	// Other Eye Location
	FVector OutOtherEyesLocation;
	FRotator UNUSED_OutOtherEyesRotation;
	Character->GetActorEyesViewPoint(OutOtherEyesLocation, UNUSED_OutOtherEyesRotation);

	// Direction To Character
	const FVector DirToCharacter = OutOtherEyesLocation - OutEyesLocation;
	const FVector DirToCharacterNormal = DirToCharacter.GetSafeNormal();

	// 2. Distance Check
	const float DistSq2D = DirToCharacter.SizeSquared2D();
	if (DistSq2D > FMath::Square(MaxSightThreshold))
	{
		// Out of Range
		return false;
	}

	// 3. Dot Product Check
	const float Dot = FVector::DotProduct(GetActorForwardVector(), DirToCharacterNormal);
	const float DotThreshold = FMath::Cos(FMath::DegreesToRadians(ConeHalfAngle));
	if (Dot < DotThreshold)
	{
		// Undetected in Vision Cone Range
		return false;
	}

	// 4. Raycast Check
	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);
	CollisionParams.AddIgnoredActor(Character);
	const bool bHit = World->LineTraceSingleByChannel(HitResult, OutEyesLocation, OutOtherEyesLocation, ECollisionChannel::ECC_Visibility, CollisionParams);
	
	// Unblocking needs to be met
	return !bHit;
}

