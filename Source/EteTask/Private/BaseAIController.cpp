#include "BaseAIController.h"
#include "BaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "NavigationSystem.h"

void ABaseAIController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	BaseCharacter = Cast<ABaseCharacter>(InPawn);
	GetWorldTimerManager().SetTimerForNextTick(this, &ABaseAIController::Track);
	GetWorldTimerManager().SetTimer(
			TrackingTimerHandle, 
			this, 
			&ABaseAIController::Track, 
			1.0f, 
			true);
}

void ABaseAIController::Track()
{
	if (bHasRequestedMove) return;
	
	ABaseCharacter* PreviousOfSameType = BaseCharacter->GetPreviousCharacter();
	FVector TargetLocation;
	if (!PreviousOfSameType)
	{
		FVector Origin = BaseCharacter->GetActorLocation();
		UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
		if (NavSys)
		{
			FNavLocation RandomLocation;
			if (NavSys->GetRandomPointInNavigableRadius(Origin, 5000.0f, RandomLocation))
			{
				TargetLocation = RandomLocation.Location;
			}
			else
			{
				TargetLocation = Origin;
			}
		}
		else
		{
			TargetLocation = Origin;
		}
	}
	else
	{
		TargetLocation = PreviousOfSameType->GetActorLocation();
	}
	
	bHasRequestedMove = true;
	LastTargetLocation = TargetLocation;

	ShowPath(TargetLocation);
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalLocation(TargetLocation);
	MoveRequest.SetAcceptanceRadius(1.1f * BaseCharacter->GetCapsuleComponent()->GetUnscaledCapsuleRadius());
	MoveRequest.SetUsePathfinding(true);
	MoveRequest.SetAllowPartialPath(true);
	MoveTo(MoveRequest);
}

void ABaseAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	UE_LOG(LogTemp, Log, TEXT("OnMoveCompleted: Code=%d, IsSuccess=%d"), (int32)Result.Code, Result.IsSuccess());
	if (Result.IsSuccess())
	{
		UE_LOG(LogTemp, Warning, TEXT("Move succes!"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Move failed!"));
	}
	bHasRequestedMove = false;
}

void ABaseAIController::ShowPath(const FVector& TargetLocation)
{
	// Optional: visualize path
	UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(GetWorld());
	if (NavSys)
	{
		FPathFindingQuery Query;
		if (BuildPathfindingQuery(TargetLocation, Query))
		{
			FPathFindingResult Result = NavSys->FindPathSync(Query, EPathFindingMode::Regular);
			if (Result.IsSuccessful() && Result.Path.IsValid())
			{
				const TArray<FNavPathPoint>& PathPoints = Result.Path->GetPathPoints();
				FColor DebugColor = FColor::MakeRandomColor();
				for (int32 i = 0; i < PathPoints.Num() - 1; ++i)
				{
					DrawDebugLine(GetWorld(), PathPoints[i].Location, PathPoints[i + 1].Location, DebugColor, false, 1.0f, 0, 5.0f);
				}
			}
		}
	}
}

