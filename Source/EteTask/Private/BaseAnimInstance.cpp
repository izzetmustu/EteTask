#include "BaseAnimInstance.h"
#include "BaseCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UBaseAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
 
	BaseCharacter = Cast<ABaseCharacter>(TryGetPawnOwner());
	if (BaseCharacter)
	{
		BaseCharacterMovement = BaseCharacter->GetCharacterMovement();
	}
}
 
void UBaseAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
 
	if (BaseCharacterMovement)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(BaseCharacterMovement->Velocity);
	}
}
