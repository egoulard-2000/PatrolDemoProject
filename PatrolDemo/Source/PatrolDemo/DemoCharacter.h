// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "DemoCharacter.generated.h"

UCLASS()
class PATROLDEMO_API ADemoCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADemoCharacter();

	bool CanBeSeen() const;

	UFUNCTION(BlueprintPure)
	FVector GetCharacterFeetLocation() const;

	UFUNCTION(BlueprintPure, Category = "Sight")
	bool CanSeeCharacter(const ACharacter* Character) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "Sight")
	float MaxSightThreshold = { 2000.f };

	UPROPERTY(EditAnywhere, Category = "Sight")
	float ConeHalfAngle = { 45.f };

	UPROPERTY(BlueprintReadOnly, Category = "Sight")
	bool bCanBeSeen = { true };
};
