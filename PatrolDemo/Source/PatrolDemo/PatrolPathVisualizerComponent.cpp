// Fill out your copyright notice in the Description page of Project Settings.

#include "PatrolPathVisualizerComponent.h"

// Sets default values for this component's properties
UPatrolPathVisualizerComponent::UPatrolPathVisualizerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UPatrolPathVisualizerComponent::BeginPlay()
{
	Super::BeginPlay();
}

