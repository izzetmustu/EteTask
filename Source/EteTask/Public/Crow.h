#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Crow.generated.h"

UCLASS()
class ETETASK_API ACrow : public ABaseCharacter
{
	GENERATED_BODY()
public:
	ACrow();
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditAnywhere, Category = "Movement")
	float DesiredFlyingHeight = 600.0f;
};
