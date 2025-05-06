#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "Human.generated.h"

UCLASS()
class ETETASK_API AHuman : public ABaseCharacter
{
	GENERATED_BODY()
public:
	AHuman();
protected:
	virtual void BeginPlay() override;
};
