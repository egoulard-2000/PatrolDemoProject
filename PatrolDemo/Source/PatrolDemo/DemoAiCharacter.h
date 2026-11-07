// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DemoCharacter.h"
#include "DemoAiCharacter.generated.h"

class ADemoAiController;
class APatrolPath;

UENUM(BlueprintType)
enum class ENonCombatBehaviorType : uint8
{
	ROAM,
	PATROL,
	IDLE,
	NONE UMETA(Hidden)
};

/**
 * 
 */
UCLASS()
class PATROLDEMO_API ADemoAiCharacter : public ADemoCharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	ADemoAiCharacter();

	UFUNCTION(BlueprintPure, Category = "AI Controller")
	ADemoAiController* GetDemoAIController() const { return DemoAiController; }

	UFUNCTION(BlueprintPure, Category = "Non-Combat Behaviors")
	ENonCombatBehaviorType GetNonCombatBehaviorType() const { return DefaultNonCombatBehaviorType; }

	UFUNCTION(BlueprintPure, Category = "Non-Combat Behaviors | Patrol")
	APatrolPath* GetPatrolPath() const { return PatrolPathPtr.Get(); }

	UFUNCTION(BlueprintPure, Category = "Non-Combat Behaviors | Patrol")
	bool IsInPatrol() const { return bInPatrol; }

	void SetIsInPatrol(const bool bIsInPatrol) { bInPatrol = bIsInPatrol; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() final;

	// Called every frame
	void Tick(float DeltaTime) final;

private:
	UPROPERTY()
	TObjectPtr<ADemoAiController> DemoAiController;

	UPROPERTY(EditAnywhere, Category = "Non-Combat Behaviors")
	ENonCombatBehaviorType DefaultNonCombatBehaviorType = { ENonCombatBehaviorType::NONE };

	UPROPERTY(EditAnywhere, Category = "Non-Combat Behaviors | Patrol", Meta = (DisplayName = "Patrol Path", EditCondition = "DefaultNonCombatBehaviorType == ENonCombatBehaviorType::PATROL", EditConditionHides))
	TWeakObjectPtr<APatrolPath> PatrolPathPtr;

	UPROPERTY(EditAnywhere, Category = "Non-Combat Behaviors | Patrol")
	float PatrolWalkSpeedMultiplier = 0.5f;

	UPROPERTY(VisibleAnywhere, Category = "Non-Combat Behaviors | Patrol")
	bool bInPatrol = { false };

	UPROPERTY()
	float CachedMovementSpeed;
};
