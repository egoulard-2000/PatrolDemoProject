// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolNode.generated.h"

class UBillboardComponent;
class UPatrolNodeVisualizerComponent;

UCLASS(Blueprintable)
class PATROLDEMO_API APatrolNode : public AActor
{
	GENERATED_BODY()

	friend class UPatrolEdMode;
	friend class UPatrolEditorObject;

public:
	// Sets default values for this actor's properties
	APatrolNode();

	UFUNCTION(BlueprintPure, Category = "Patrol Node")
	APatrolNode* GetNextNode() const { return NextNode; }

	UFUNCTION(BlueprintPure, Category = "Patrol Node")
	APatrolNode* GetPrevNode() const { return PrevNode; }

	UFUNCTION(BlueprintPure, Category = "Patrol Node")
	float GetMinWaitTime() const { return WaitTime.Min; }

	UFUNCTION(BlueprintPure, Category = "Patrol Node")
	float GetMaxWaitTime() const { return WaitTime.Max; }

	UFUNCTION(BlueprintPure, Category = "Patrol Node")
	float GetArrivalRadius() const { return ArrivalRadius; }

	UFUNCTION(BlueprintPure, Category = "Patrol Node")
	bool IsLastNode() const { return NextNode == nullptr; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() final;

	void Tick(float DeltaTime) final;

	bool ShouldTickIfViewportsOnly() const final { return true; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sprite")
	TObjectPtr<UBillboardComponent> SpriteComponent;

#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UPatrolNodeVisualizerComponent> VisualizerComponent;
#endif

	// The wait time at this node upon arrival (Min and Max values are used to randomize the wait time)
	UPROPERTY(EditAnywhere, Category = "Path Settings | Patrol Node", Meta = (ClampMin = "0.0", UIMin = "0.0"))
	FFloatInterval WaitTime = { FFloatInterval(1.f, 10.f) };

	// The radius of tolerance at which a pawn considers themselves "arrived" at this node
	UPROPERTY(EditAnywhere, Category = "Path Settings | Patrol Node", Meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ArrivalRadius = { 100.f };

private:
	UPROPERTY(EditAnywhere, Category = "Path Settings | Next Node")
	TObjectPtr<APatrolNode> NextNode;

	// We only care to place nodes sequentially, so we don't need to expose this to the editor
	UPROPERTY()
	TObjectPtr<APatrolNode> PrevNode;

	void SetNextNode(APatrolNode* const NewNextNode) { NextNode = NewNextNode; }
	void SetPrevNode(APatrolNode* const NewPrevNode) { PrevNode = NewPrevNode; }
};
