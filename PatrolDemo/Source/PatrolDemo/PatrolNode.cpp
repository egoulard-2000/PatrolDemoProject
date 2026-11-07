// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolNode.h"
#include "Components/BillboardComponent.h"
#include "PatrolNodeVisualizerComponent.h"

APatrolNode::APatrolNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITOR
	// Setup Visualizer Component
	VisualizerComponent = CreateEditorOnlyDefaultSubobject<UPatrolNodeVisualizerComponent>(TEXT("Visualizer"));
	VisualizerComponent->SetComponentTickEnabled(false);
#endif

	// Setup Sprite Component (No interactive gameplay uses)
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	SpriteComponent->SetComponentTickEnabled(false);
	ConstructorHelpers::FObjectFinderOptional<UTexture2D> TargetPointTextureObj(TEXT("/Engine/EditorResources/S_TargetPoint"));
	if (UTexture2D* const TargetPointTexture = TargetPointTextureObj.Get())
	{
		SpriteComponent->SetSprite(TargetPointTexture);
	}
	SpriteComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrolNode::BeginPlay()
{
	Super::BeginPlay();
}

void APatrolNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if 0 // Replaced by new BP constructed visualizer
	UWorld* const World = GetWorld();
	if (NextNode && World && World->WorldType == EWorldType::Editor)
	{
		constexpr float LineWeight = 25.f;
		constexpr float ArrowSize = 100.f;
		DrawDebugDirectionalArrow(World, GetActorLocation(), NextNode->GetActorLocation(), ArrowSize, FColor::Green, false, -1.f, 0, LineWeight);
	}
#endif
}

