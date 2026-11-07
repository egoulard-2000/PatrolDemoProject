#include "DemoAiController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardData.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "DemoAiCharacter.h"

DEFINE_LOG_CATEGORY(LogDemoAIController);

void ADemoAiController::BeginPlay()
{
	Super::BeginPlay();

	RunBehaviorTree(DemoAIBehaviorTree);
}

void ADemoAiController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	DemoAiCharacterPtr = Cast<ADemoAiCharacter>(InPawn);
}

bool ADemoAiController::RunBehaviorTree(UBehaviorTree* BTAsset)
{
	if (!BTAsset)
	{
		UE_LOG(LogDemoAIController, Warning, TEXT("Invalid Behavior Tree"));
		return false;
	}

	return Super::RunBehaviorTree(BTAsset);
}