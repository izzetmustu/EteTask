#include "BaseCharacter.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseAIController.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	PreviousOfSameType = nullptr;
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ABaseAIController::StaticClass();
	bCanAffectNavigationGeneration = true;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PossibleMaterials.Num() > 0)
	{
		int32 RandomIndex = FMath::RandRange(0, PossibleMaterials.Num() - 1);
		GetMesh()->SetMaterial(0, PossibleMaterials[RandomIndex]);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No materials available for the character!"));
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABaseCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
