#include "Drone.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SceneComponent.h"

ADrone::ADrone()
{
	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 400.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
}

void ADrone::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
}

void ADrone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector CapsuleLocation = GetActorLocation();
	FVector MeshOffset = FVector(0.0f, 0.0f, DesiredFlyingHeight);
	GetMesh()->SetWorldLocation(CapsuleLocation + MeshOffset);
}
