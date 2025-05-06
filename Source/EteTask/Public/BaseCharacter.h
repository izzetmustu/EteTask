#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ETETASK_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	ABaseCharacter();
protected:
	UPROPERTY()
	ABaseCharacter* PreviousOfSameType;
	virtual void BeginPlay() override;
	UPROPERTY(EditAnywhere, Category = "Material")
	TArray<UMaterialInterface*> PossibleMaterials;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
public:	
	FORCEINLINE ABaseCharacter* GetPreviousCharacter() const { return PreviousOfSameType; }
	FORCEINLINE void SetPreviousCharacter(ABaseCharacter* Previous) { PreviousOfSameType = Previous; }
};
