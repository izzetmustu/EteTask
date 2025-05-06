#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EteTaskGameModeBase.generated.h"

class AGroundManager;
class ABaseCharacter;

UCLASS()
class ETETASK_API AEteTaskGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	FTimerHandle CharacterSpawnTimerHandle;
	int32 CurrentSpawnIndex = 0;
	TArray<ABaseCharacter*> Characters;
	TArray<TSubclassOf<ABaseCharacter>> CharacterTypes;
	
	UPROPERTY()
	AGroundManager* GroundManager;
public:
	AEteTaskGameModeBase();
	void SpawnCharactersAtCenters();
	void SpawnNextCharacter();
};
