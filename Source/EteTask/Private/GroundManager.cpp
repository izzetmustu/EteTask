#include "GroundManager.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "NavigationSystem.h"
#include "Components/BrushComponent.h"
#include "DrawDebugHelpers.h"
#include "EngineUtils.h"

AGroundManager::AGroundManager()
{
	PrimaryActorTick.bCanEverTick = false;

	GroundHISMC = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("GroundHISMC"));
	RootComponent = GroundHISMC;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Cube"));
	if (MeshAsset.Succeeded())
	{
		GroundMesh = MeshAsset.Object;
		GroundHISMC->SetStaticMesh(GroundMesh);
	}

	GroundHISMC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GroundHISMC->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GroundHISMC->SetGenerateOverlapEvents(true);
	GroundHISMC->SetMobility(EComponentMobility::Static);
	GroundHISMC->bNavigationRelevant = true;
}

void AGroundManager::BeginPlay()
{
	Super::BeginPlay();
	
	GenerateGroundTiles();
	BuildNavigationSystem();

#if WITH_EDITOR
	FNavigationSystem::AddNavigationSystemToWorld(*GetWorld(), FNavigationSystemRunMode::GameMode);
#endif
}

void AGroundManager::GenerateGroundTiles()
{
	if (!GroundMesh) return;
	const FVector TileScale = FVector(TileSizeX / 100.f, TileSizeY / 100.f, 1.f); // Default is 100x100
	for (int32 x = 0; x < GridSizeX; ++x)
	{
		for (int32 y = 0; y < GridSizeY; ++y)
		{
			FVector Location = FVector(x * TileSizeX + TileSizeX / 2.f, y * TileSizeY + TileSizeY / 2.f, 0.0f);
			FTransform Transform(FRotator::ZeroRotator, Location, TileScale);
			GroundHISMC->AddInstance(Transform);

			GroundCenters.Add(Location);
		}
	}
	GroundHISMC->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GroundHISMC->SetCanEverAffectNavigation(true);
	GroundHISMC->MarkRenderStateDirty();
}

void AGroundManager::BuildNavigationSystem()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	for (TActorIterator<ANavMeshBoundsVolume> It(World); It; ++It)
	{
		ANavMeshBoundsVolume* NavVolume = *It;
		if (NavVolume && NavVolume->GetBrushComponent())
		{
			NavVolume->GetBrushComponent()->SetMobility(EComponentMobility::Movable);
			NavVolume->SetActorLocation(FVector(TotalSize / 2.f, TotalSize / 2.f, 0.0f));
			NavVolume->GetBrushComponent()->Bounds.BoxExtent = FVector(TotalSize / 2.f, TotalSize / 2.f, 3000.0f);
			
			FVector Origin = NavVolume->GetActorLocation();
			FVector Extent = NavVolume->GetBrushComponent()->Bounds.BoxExtent;
			// Draw the debug box
			DrawDebugBox(World, Origin, Extent, FColor::Red, true, -1.f, 0, 20.0f);

			if (UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()))
			{
				UE_LOG(LogTemp, Warning, TEXT("NAVSYS BUILDING"));
				NavSys->OnNavigationBoundsUpdated(NavVolume);
				NavSys->Build();
			}
		}
	}
}
