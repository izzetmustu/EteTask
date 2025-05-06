#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

class ABaseCharacter;

UCLASS()
class ETETASK_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
protected:
	bool bHasRequestedMove = false;
	FVector LastTargetLocation;
	FTimerHandle TrackingTimerHandle;
	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void ShowPath(const FVector& TargetLocation);
	UPROPERTY()
	ABaseCharacter* BaseCharacter;
public:
	void Track();
};
