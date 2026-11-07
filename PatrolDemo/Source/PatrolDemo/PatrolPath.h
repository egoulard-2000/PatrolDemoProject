// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PatrolPath.generated.h"

class APatrolNode;
class UBillboardComponent;
class UBoxComponent;
class UPatrolPathVisualizerComponent;

UCLASS()
class PATROLDEMO_API APatrolPath : public AActor
{
	GENERATED_BODY()
	
	friend class FPatrolPathComponentVisualizer;

public:	
	// Sets default values for this actor's properties
	APatrolPath();

	void AddPatrolNode(APatrolNode* const PatrolNode);
	void AddPatrolNode(APatrolNode* const PatrolNode, const int32 Index);
	void ClearPath();
	void FinalizePath();
	bool IsEmpty() const { return PatrolPathNodes.IsEmpty(); }
	int32 GetSize() const { return PatrolPathNodes.Num(); }

	UFUNCTION(BlueprintPure, Category = "Patrol Path | Node")
	APatrolNode* GetFirstNodeInPath() const { return PatrolPathNodes[0]; }

	UFUNCTION(BlueprintPure, Category = "Patrol Path | Node")
	APatrolNode* GetLastNodeInPath() const { return PatrolPathNodes[PatrolPathNodes.Num() - 1]; }

protected:
	// Called when the game starts or when spawned
	void BeginPlay() final;

	// Called every frame
	void Tick(float DeltaTime) final;

	// Creates the visual around the path based on the placement of patrol nodes
	void OutputAroundPath(FVector& OutCenter, FVector& OutExtents);

private:
#if WITH_EDITORONLY_DATA
	UPROPERTY()
	TObjectPtr<UPatrolPathVisualizerComponent> VisualizerComponent;
#endif

	UPROPERTY(EditAnywhere, Category = "Sprite")
	TObjectPtr<UBillboardComponent> SpriteComponent;

	UPROPERTY()
	TArray<TObjectPtr<APatrolNode>> PatrolPathNodes;
};
