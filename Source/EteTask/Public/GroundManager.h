#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/HierarchicalInstancedStaticMeshComponent.h"
#include "GroundManager.generated.h"

class ABaseCharacter;

UCLASS()
class ETETASK_API AGroundManager : public AActor
{
	GENERATED_BODY()
	
public:
	AGroundManager();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, Category = "Ground")
	UHierarchicalInstancedStaticMeshComponent* GroundHISMC;
	TArray<FVector> GroundCenters;

	UPROPERTY(EditAnywhere, Category = "Ground")
	int32 GridSizeX = 6;
	UPROPERTY(EditAnywhere, Category = "Ground")
	int32 GridSizeY{6};
	UPROPERTY(VisibleAnywhere, Category = "Ground")
	float TotalSize = 10000.0f; // 100 meters = 10000 cm
	UPROPERTY(VisibleAnywhere, Category = "Ground")
	float TileSizeX = TotalSize / GridSizeX;
	UPROPERTY(VisibleAnywhere, Category = "Ground")
	float TileSizeY = TotalSize / GridSizeY;
	
	UPROPERTY(EditAnywhere, Category = "Ground")
	UStaticMesh* GroundMesh;
public:
	void GenerateGroundTiles();
	void BuildNavigationSystem();
	FORCEINLINE UHierarchicalInstancedStaticMeshComponent* GetGroundHISMC() const { return GroundHISMC; }
	FORCEINLINE TArray<FVector> GetGroundCenters() const { return GroundCenters; }
};
