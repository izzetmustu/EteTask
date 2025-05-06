#include "EteTaskGameModeBase.h"
#include "GroundManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "BaseCharacter.h"
#include "Human.h"
#include "Crow.h"
#include "Robot.h"
#include "Drone.h"
#include "Engine/World.h"
#include "Engine/StaticMesh.h"
#include "DrawDebugHelpers.h"

AEteTaskGameModeBase::AEteTaskGameModeBase()
{
	static ConstructorHelpers::FClassFinder<ACharacter> HumanBPClass(TEXT("/Game/Blueprints/Characters/BP_Human.BP_Human_C"));
	if (HumanBPClass.Class != nullptr)
	{
		CharacterTypes.Add(TSubclassOf<AHuman>(HumanBPClass.Class));
	}
	static ConstructorHelpers::FClassFinder<ACharacter> CrowBPClass(TEXT("/Game/Blueprints/Characters/BP_Crow.BP_Crow_C"));
	if (CrowBPClass.Class != nullptr)
	{
		CharacterTypes.Add(TSubclassOf<ACrow>(CrowBPClass.Class));
	}
	static ConstructorHelpers::FClassFinder<ACharacter> RobotBPClass(TEXT("/Game/Blueprints/Characters/BP_Robot.BP_Robot_C"));
	if (RobotBPClass.Class != nullptr)
	{
		CharacterTypes.Add(TSubclassOf<ARobot>(RobotBPClass.Class));
	}
	static ConstructorHelpers::FClassFinder<ACharacter> DroneBPClass(TEXT("/Game/Blueprints/Characters/BP_Drone.BP_Drone_C"));
	if (DroneBPClass.Class != nullptr)
	{
		CharacterTypes.Add(TSubclassOf<ADrone>(DroneBPClass.Class));
	}
}

void AEteTaskGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	GroundManager = GetWorld()->SpawnActor<AGroundManager>(AGroundManager::StaticClass(), FVector::ZeroVector, FRotator::ZeroRotator);
	if (GroundManager)
	{
		SpawnCharactersAtCenters();
	}
}

void AEteTaskGameModeBase::SpawnCharactersAtCenters()
{
	if (!GetWorld()) return;
	GetWorld()->GetTimerManager().SetTimer(CharacterSpawnTimerHandle, this, &AEteTaskGameModeBase::SpawnNextCharacter, 0.1f, true);
}

void AEteTaskGameModeBase::SpawnNextCharacter()
{
	int32 NumCharacterTypes = CharacterTypes.Num();
	if (NumCharacterTypes == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(CharacterSpawnTimerHandle);
		return;
	}

	FTransform TileTransform;
	if (!GroundManager->GetGroundHISMC()->GetInstanceTransform(CurrentSpawnIndex, TileTransform, true))
	{
		++CurrentSpawnIndex;
		return;
	}

	// Pick character class
	TSubclassOf<ABaseCharacter> CharacterClass = CharacterTypes[CurrentSpawnIndex % NumCharacterTypes];
	if (!CharacterClass)
	{
		++CurrentSpawnIndex;
		return;
	}

	ABaseCharacter* Character = GetWorld()->SpawnActorDeferred<ABaseCharacter>(CharacterClass, TileTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
	if (!Character)
	{
		++CurrentSpawnIndex;
		return;
	}

	float VerticalOffset = 0.f;
	FVector TileCenter = TileTransform.GetLocation();
	float TileTopZ = TileCenter.Z + (50.f * TileTransform.GetScale3D().Z);
	TileCenter.Z = TileTopZ + VerticalOffset;

	Character->SetActorLocation(TileCenter);
	UGameplayStatics::FinishSpawningActor(Character, FTransform(TileTransform.GetRotation(), TileCenter));
	Characters.Add(Character);
	int PreviousIndex = CurrentSpawnIndex - 4;
	if (PreviousIndex >= 0)
	{
		Character->SetPreviousCharacter(Characters[PreviousIndex]); // Placeholder
	}
	else
	{
		Character->SetPreviousCharacter(nullptr);
	}

	++CurrentSpawnIndex;

	if (CurrentSpawnIndex >= GroundManager->GetGroundHISMC()->GetInstanceCount())
	{
		GetWorld()->GetTimerManager().ClearTimer(CharacterSpawnTimerHandle);
	}
}
