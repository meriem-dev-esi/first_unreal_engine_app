#include "CombatParkourMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TimerManager.h"

UCombatParkourMovementComponent::UCombatParkourMovementComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCombatParkourMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	CharacterOwner = Cast<ACharacter>(GetOwner());
	if (CharacterOwner)
	{
		MovementComp = CharacterOwner->GetCharacterMovement();
	}
}

void UCombatParkourMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (MovementComp && MovementComp->IsMovingOnGround())
	{
		// Reset aerial abilities when landing
		if (!bCanDoubleJump || !bCanAirDash)
		{
			bCanDoubleJump = true;
			bCanAirDash = true;
			if (CurrentParkourState != ECombatParkourState::Sliding)
			{
				CurrentParkourState = ECombatParkourState::Grounded;
				OnParkourStateChanged.Broadcast(CurrentParkourState);
			}
		}
	}
}

bool UCombatParkourMovementComponent::TryDoubleJump()
{
	if (!CharacterOwner || !MovementComp) return false;

	// Must be airborne and have double jump available
	if (!MovementComp->IsMovingOnGround() && bCanDoubleJump)
	{
		bCanDoubleJump = false;
		CurrentParkourState = ECombatParkourState::DoubleJumping;
		OnParkourStateChanged.Broadcast(CurrentParkourState);

		FVector JumpVelocity = CharacterOwner->GetVelocity();
		JumpVelocity.Z = DoubleJumpForce;
		MovementComp->Velocity = JumpVelocity;

		return true;
	}
	return false;
}

bool UCombatParkourMovementComponent::TryAirDash(FVector InputDirection)
{
	if (!CharacterOwner || !MovementComp) return false;

	if (!MovementComp->IsMovingOnGround() && bCanAirDash)
	{
		bCanAirDash = false;
		CurrentParkourState = ECombatParkourState::AirDashing;
		OnParkourStateChanged.Broadcast(CurrentParkourState);

		FVector DashDir = InputDirection.IsNearlyZero() ? CharacterOwner->GetActorForwardVector() : InputDirection.GetSafeNormal();
		DashDir.Z = 0.1f;
		DashDir.Normalize();

		MovementComp->Velocity = DashDir * AirDashImpulse;

		return true;
	}
	return false;
}

bool UCombatParkourMovementComponent::StartSlide()
{
	if (!CharacterOwner || !MovementComp) return false;

	if (MovementComp->IsMovingOnGround() && CurrentParkourState != ECombatParkourState::Sliding)
	{
		CurrentParkourState = ECombatParkourState::Sliding;
		OnParkourStateChanged.Broadcast(CurrentParkourState);

		// Lower ground friction and launch forward
		MovementComp->GroundFriction = 0.2f;
		MovementComp->BrakingDecelerationWalking = 50.0f;

		FVector Forward = CharacterOwner->GetActorForwardVector();
		CharacterOwner->LaunchCharacter(Forward * SlideInitialBoost, true, false);

		if (UWorld* World = GetWorld())
		{
			World->GetTimerManager().SetTimer(SlideTimerHandle, this, &UCombatParkourMovementComponent::StopSlide, SlideDurationSeconds, false);
		}
		return true;
	}
	return false;
}

void UCombatParkourMovementComponent::StopSlide()
{
	if (CurrentParkourState != ECombatParkourState::Sliding) return;

	CurrentParkourState = ECombatParkourState::Grounded;
	OnParkourStateChanged.Broadcast(CurrentParkourState);

	if (MovementComp)
	{
		// Restore normal friction and braking
		MovementComp->GroundFriction = 8.0f;
		MovementComp->BrakingDecelerationWalking = 2048.0f;
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(SlideTimerHandle);
	}
}
