#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseAnimInstance.generated.h"

class ABaseCharacter;
UCLASS()
class ETETASK_API UBaseAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	
	UPROPERTY(BlueprintReadOnly)
	ABaseCharacter* BaseCharacter;
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	class UCharacterMovementComponent* BaseCharacterMovement{nullptr};
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaTime) override;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
};
