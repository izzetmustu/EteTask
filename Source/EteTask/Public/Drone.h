#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Drone.generated.h"

UCLASS()
class ETETASK_API ADrone : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ADrone();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DesiredFlyingHeight = 400.0f;
};
