// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "DemoAiController.generated.h"

class ADemoAiCharacter;

DECLARE_LOG_CATEGORY_EXTERN(LogDemoAIController, Log, All);

/**
 * 
 */
UCLASS()
class PATROLDEMO_API ADemoAiController : public AAIController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Demo AI Character")
	ADemoAiCharacter* GetDemoAICharacter() const { return DemoAiCharacterPtr.Get(); }

	bool RunBehaviorTree(UBehaviorTree* BTAsset) final;

protected:
	void BeginPlay() final;
	void OnPossess(APawn* InPawn) final;

private:
	UPROPERTY()
	TWeakObjectPtr<ADemoAiCharacter> DemoAiCharacterPtr;

	UPROPERTY(EditAnywhere, Category = "Demo | Behavior Tree")
	TObjectPtr<UBehaviorTree> DemoAIBehaviorTree;
};
