#include "PatrolPath.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "PatrolNode.h"
#include "PatrolPathVisualizerComponent.h"

// Console Vars
static TAutoConsoleVariable<int32> CVarPatrolShowPatrolNodes(
	TEXT("AI.Debug.Patrol.ShowAllPatrolNodes"),
	0,
	TEXT("Toggles debug drawing for the AI patrol routes <0/1>\n")
	TEXT("0: Disabled\n")
	TEXT("1: Enabled"),
	ECVF_Cheat
);

// Sets default values
APatrolPath::APatrolPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

#if WITH_EDITOR
	// Setup Visualizer Component
	VisualizerComponent = CreateEditorOnlyDefaultSubobject<UPatrolPathVisualizerComponent>(TEXT("Visualizer"));
	VisualizerComponent->SetComponentTickEnabled(false);
#endif

	// Sprite Setup (No interactive gameplay uses)
	SpriteComponent = CreateDefaultSubobject<UBillboardComponent>(TEXT("Sprite"));
	SpriteComponent->SetComponentTickEnabled(false);
	ConstructorHelpers::FObjectFinderOptional<UTexture2D> StickFigureTextureObj(TEXT("/Game/silhouette_stickfigure_512"));
	if (UTexture2D* const StickFigureTexture = StickFigureTextureObj.Get())
	{
		SpriteComponent->SetSprite(StickFigureTexture);
	}
	SpriteComponent->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void APatrolPath::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APatrolPath::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APatrolPath::AddPatrolNode(APatrolNode* const PatrolNode)
{
	PatrolPathNodes.Add(PatrolNode);
}

void APatrolPath::AddPatrolNode(APatrolNode* const PatrolNode, const int32 Index)
{
	PatrolPathNodes.Insert(PatrolNode, Index);
}

void APatrolPath::ClearPath()
{
	PatrolPathNodes.Empty();
}

void APatrolPath::FinalizePath()
{
	FVector OutCenter, OutBoxExtents;
	OutputAroundPath(OutCenter, OutBoxExtents);

	const FVector Center = OutCenter + (FVector::UpVector * 100.f);
	SetActorLocation(Center);
}

void APatrolPath::OutputAroundPath(FVector& OutCenter, FVector& OutExtents)
{
	if (PatrolPathNodes.IsEmpty())
	{
		return;
	}

	float MinX = FLT_MAX, MinY = FLT_MAX, MinZ = FLT_MAX;
	float MaxX = -FLT_MAX, MaxY = -FLT_MAX, MaxZ = -FLT_MAX;

	float CenterX = 0.f, CenterY = 0.f, CenterZ = 0.f;
	float ExtentX = 0.f, ExtentY = 0.f, ExtentZ = 0.f;

	for (const APatrolNode* Node : PatrolPathNodes)
	{
		if (!Node)
		{
			continue;
		}

		const FVector NodeLocation = Node->GetActorLocation();
		MinX = FMath::Min(MinX, NodeLocation.X);
		MaxX = FMath::Max(MaxX, NodeLocation.X);

		MinY = FMath::Min(MinY, NodeLocation.Y);
		MaxY = FMath::Max(MaxY, NodeLocation.Y);

		MinZ = FMath::Min(MinZ, NodeLocation.Z);
		MaxZ = FMath::Max(MaxZ, NodeLocation.Z);

		CenterX = (MaxX + MinX) * 0.5f;
		CenterY = (MaxY + MinY) * 0.5f;
		CenterZ = (MaxZ + MinZ) * 0.5f;

		ExtentX = (MaxX - MinX) * 0.5f;
		ExtentY = (MaxY - MinY) * 0.5f;
		ExtentZ = (MaxZ - MinZ) * 0.5f;
	}

	OutCenter = FVector(CenterX, CenterY, CenterZ);
	constexpr float ExtentBuffer = 1.2f;
	OutExtents = FVector(ExtentX * ExtentBuffer, ExtentY * ExtentBuffer, ExtentZ * ExtentBuffer);
}

