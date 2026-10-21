#include "FighterCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/DamageEvents.h"
#include "TimerManager.h"
#include "TacticalTimeDilationComponent.h"
#include "CombatInventoryComponent.h"
#include "CombatParkourMovementComponent.h"
#include "CombatRadarComponent.h"

AFighterCharacter::AFighterCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Character movement setup for 3D brawler
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->MaxWalkSpeed = 600.f;

	// Camera setup
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 40.0f, 60.0f);
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Initialize Tactical Subsystems
	TimeDilationComp = CreateDefaultSubobject<UTacticalTimeDilationComponent>(TEXT("TimeDilationComp"));
	InventoryComp = CreateDefaultSubobject<UCombatInventoryComponent>(TEXT("InventoryComp"));
	ParkourComp = CreateDefaultSubobject<UCombatParkourMovementComponent>(TEXT("ParkourComp"));
	RadarComp = CreateDefaultSubobject<UCombatRadarComponent>(TEXT("RadarComp"));

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	CombatState = EFighterCombatState::Idle;
}

void AFighterCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	CurrentStamina = MaxStamina;
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);
	OnStaminaChanged(CurrentStamina, MaxStamina);
}

void AFighterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Stamina regeneration when not attacking or blocking
	if (CombatState == EFighterCombatState::Idle)
	{
		if (CurrentStamina < MaxStamina)
		{
			CurrentStamina = FMath::Clamp(CurrentStamina + (StaminaRegenRate * DeltaTime), 0.0f, MaxStamina);
			OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);
		}
	}

	// Update camera/orientation when locked on to opponent
	if (bIsLockedOn && TargetActor)
	{
		UpdateTargetLock(DeltaTime);
	}
}

void AFighterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AFighterCharacter::SetCombatState(EFighterCombatState NewState)
{
	if (CombatState == EFighterCombatState::Dead)
	{
		return;
	}

	CombatState = NewState;
	OnCombatStateChanged.Broadcast(CombatState);
}

void AFighterCharacter::LightAttack()
{
	if (CombatState != EFighterCombatState::Idle && CombatState != EFighterCombatState::Attacking)
	{
		return;
	}

	if (CurrentStamina < LightAttackStaminaCost)
	{
		return;
	}

	if (LightAttackMontages.Num() == 0)
	{
		return;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina - LightAttackStaminaCost, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

	SetCombatState(EFighterCombatState::Attacking);

	// Select montage based on combo index
	int32 AttackIdx = ComboIndex % LightAttackMontages.Num();
	UAnimMontage* MontageToPlay = LightAttackMontages[AttackIdx];

	if (MontageToPlay)
	{
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MontageToPlay);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AFighterCharacter::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
		}
	}

	ComboIndex++;

	// Reset combo after inactivity window
	GetWorldTimerManager().ClearTimer(ComboResetTimer);
	GetWorldTimerManager().SetTimer(ComboResetTimer, this, &AFighterCharacter::ResetCombo, 1.5f, false);
}

void AFighterCharacter::HeavyAttack()
{
	if (CombatState != EFighterCombatState::Idle)
	{
		return;
	}

	if (CurrentStamina < HeavyAttackStaminaCost)
	{
		return;
	}

	if (HeavyAttackMontages.Num() == 0)
	{
		return;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina - HeavyAttackStaminaCost, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

	SetCombatState(EFighterCombatState::Attacking);

	int32 AttackIdx = FMath::RandRange(0, HeavyAttackMontages.Num() - 1);
	UAnimMontage* MontageToPlay = HeavyAttackMontages[AttackIdx];

	if (MontageToPlay)
	{
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(MontageToPlay);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AFighterCharacter::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, MontageToPlay);
		}
	}

	ResetCombo();
}

void AFighterCharacter::SuperAttack()
{
	if (CombatState == EFighterCombatState::Dead || !bCanUseUltimate)
	{
		return;
	}

	if (CurrentStamina < SuperAttackStaminaCost)
	{
		return;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina - SuperAttackStaminaCost, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

	bCanUseUltimate = false;
	OnUltimateTriggered.Broadcast(UltimateCooldownSeconds);

	// Start cooldown timer
	GetWorldTimerManager().SetTimer(UltimateCooldownTimer, this, &AFighterCharacter::ResetUltimateCooldown, UltimateCooldownSeconds, false);

	SetCombatState(EFighterCombatState::Attacking);

	// Perform explosive radial shockwave
	const FVector BlastOrigin = GetActorLocation();

	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(UltimateRadius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	GetWorld()->SweepMultiByChannel(HitResults, BlastOrigin, BlastOrigin + FVector(0.0f, 0.0f, 10.0f), FQuat::Identity, ECC_Pawn, SphereShape, Params);

	// Spawn persistent Hellfire hazard field at ground center
	if (HellfireHazardClass && GetWorld())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		GetWorld()->SpawnActor<AActor>(HellfireHazardClass, BlastOrigin, FRotator::ZeroRotator, SpawnParams);
	}

	TArray<AActor*> AffectedActors;

	for (const FHitResult& Hit : HitResults)
	{
		AActor* HitActor = Hit.GetActor();
		if (HitActor && HitActor != this && !AffectedActors.Contains(HitActor))
		{
			AffectedActors.Add(HitActor);

			// Deal heavy ultimate damage
			FDamageEvent DamageEvent;
			HitActor->TakeDamage(UltimateDamage, DamageEvent, GetController(), this);

			// Ignite enemy into blazing flames
			if (bUltimateIgnitesEnemies)
			{
				// Apply burning damage over time
				FDamageEvent BurnEvent;
				HitActor->TakeDamage(UltimateBurnDPS, BurnEvent, GetController(), this);
			}

			// Launch enemies into the air with radial force
			if (ACharacter* EnemyChar = Cast<ACharacter>(HitActor))
			{
				FVector LaunchDirection = (HitActor->GetActorLocation() - BlastOrigin).GetSafeNormal();
				LaunchDirection.Z = 0.6f;
				LaunchDirection.Normalize();

				EnemyChar->LaunchCharacter(LaunchDirection * UltimateLaunchForce + FVector(0.0f, 0.0f, 600.0f), true, true);
			}
		}
	}

	// Reset state after blast
	GetWorldTimerManager().SetTimer(ComboResetTimer, this, &AFighterCharacter::ResetCombo, 0.8f, false);
}

void AFighterCharacter::ResetUltimateCooldown()
{
	bCanUseUltimate = true;
	OnUltimateReady.Broadcast();
}

void AFighterCharacter::StartBlock()
{
	if (CombatState == EFighterCombatState::Idle && CurrentStamina > 10.0f)
	{
		SetCombatState(EFighterCombatState::Blocking);
		GetCharacterMovement()->MaxWalkSpeed = 250.0f;
	}
}

void AFighterCharacter::StopBlock()
{
	if (CombatState == EFighterCombatState::Blocking)
	{
		SetCombatState(EFighterCombatState::Idle);
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void AFighterCharacter::Dodge()
{
	if (CombatState != EFighterCombatState::Idle || CurrentStamina < 20.0f)
	{
		return;
	}

	CurrentStamina = FMath::Clamp(CurrentStamina - 20.0f, 0.0f, MaxStamina);
	OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

	SetCombatState(EFighterCombatState::Attacking);

	if (DodgeMontage)
	{
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DodgeMontage);

			FOnMontageEnded EndDelegate;
			EndDelegate.BindUObject(this, &AFighterCharacter::OnMontageEnded);
			AnimInstance->Montage_SetEndDelegate(EndDelegate, DodgeMontage);
		}
	}
	else
	{
		SetCombatState(EFighterCombatState::Idle);
	}
}

void AFighterCharacter::ToggleLockOn()
{
	if (bIsLockedOn)
	{
		bIsLockedOn = false;
		TargetActor = nullptr;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		OnLockOnTargetChanged.Broadcast(nullptr);
	}
	else
	{
		TargetActor = FindBestLockOnTarget();
		if (TargetActor)
		{
			bIsLockedOn = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
			OnLockOnTargetChanged.Broadcast(TargetActor);
		}
	}
}

AActor* AFighterCharacter::FindBestLockOnTarget()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFighterCharacter::StaticClass(), FoundActors);

	AActor* BestTarget = nullptr;
	float ClosestDistanceSq = FMath::Square(LockOnRange);

	const FVector CameraLocation = FollowCamera->GetComponentLocation();
	const FVector CameraForward = FollowCamera->GetForwardVector();

	for (AActor* Candidate : FoundActors)
	{
		if (Candidate == this)
		{
			continue;
		}

		AFighterCharacter* OtherFighter = Cast<AFighterCharacter>(Candidate);
		if (OtherFighter && OtherFighter->GetCombatState() == EFighterCombatState::Dead)
		{
			continue;
		}

		FVector ToTarget = Candidate->GetActorLocation() - CameraLocation;
		float DistSq = ToTarget.SizeSquared();

		if (DistSq < ClosestDistanceSq)
		{
			ToTarget.Normalize();
			float Dot = FVector::DotProduct(CameraForward, ToTarget);
			// Target must be in front of camera
			if (Dot > 0.4f)
			{
				ClosestDistanceSq = DistSq;
				BestTarget = Candidate;
			}
		}
	}

	return BestTarget;
}

void AFighterCharacter::UpdateTargetLock(float DeltaTime)
{
	if (!TargetActor)
	{
		ToggleLockOn();
		return;
	}

	float DistSq = FVector::DistSquared(GetActorLocation(), TargetActor->GetActorLocation());
	if (DistSq > FMath::Square(LockOnRange * 1.2f))
	{
		ToggleLockOn();
		return;
	}

	// Rotate character toward target
	FRotator LookRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetActor->GetActorLocation());
	LookRot.Pitch = 0.0f;
	LookRot.Roll = 0.0f;

	SetActorRotation(FMath::RInterpTo(GetActorRotation(), LookRot, DeltaTime, 10.0f));

	// Softly steer controller/camera towards target
	if (AController* MyController = GetController())
	{
		FRotator TargetCamRot = UKismetMathLibrary::FindLookAtRotation(FollowCamera->GetComponentLocation(), TargetActor->GetActorLocation() + FVector(0,0,40.f));
		FRotator NewCamRot = FMath::RInterpTo(MyController->GetControlRotation(), TargetCamRot, DeltaTime, 5.0f);
		NewCamRot.Roll = 0.0f;
		MyController->SetControlRotation(NewCamRot);
	}
}

bool AFighterCharacter::PerformMeleeHitCheck(float Damage, float Radius, float ForwardOffset)
{
	const FVector Start = GetActorLocation() + (GetActorForwardVector() * ForwardOffset) + FVector(0.0f, 0.0f, 20.0f);
	const FVector End = Start + (GetActorForwardVector() * 50.0f);

	TArray<FHitResult> HitResults;
	FCollisionShape SphereShape = FCollisionShape::MakeSphere(Radius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	bool bHit = GetWorld()->SweepMultiByChannel(HitResults, Start, End, FQuat::Identity, ECC_Pawn, SphereShape, Params);

	bool bDealtDamage = false;
	TArray<AActor*> DamagedActors;

	if (bHit)
	{
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (HitActor && HitActor != this && !DamagedActors.Contains(HitActor))
			{
				DamagedActors.Add(HitActor);

				FDamageEvent DamageEvent;
				HitActor->TakeDamage(Damage, DamageEvent, GetController(), this);
				bDealtDamage = true;
			}
		}
	}

	return bDealtDamage;
}

float AFighterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CombatState == EFighterCombatState::Dead)
	{
		return 0.0f;
	}

	float ActualDamage = DamageAmount;

	// Blocking mechanics
	if (CombatState == EFighterCombatState::Blocking)
	{
		ActualDamage *= (1.0f - BlockDamageReduction);

		// Drains stamina on block
		CurrentStamina = FMath::Clamp(CurrentStamina - (DamageAmount * 0.5f), 0.0f, MaxStamina);
		OnStaminaChanged.Broadcast(CurrentStamina, MaxStamina);

		// Guard break if stamina depleted
		if (CurrentStamina <= 0.0f)
		{
			StopBlock();
			SetCombatState(EFighterCombatState::HitStun);
		}
	}
	else
	{
		SetCombatState(EFighterCombatState::HitStun);

		if (HitReactMontage)
		{
			UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(HitReactMontage);

				FOnMontageEnded EndDelegate;
				EndDelegate.BindUObject(this, &AFighterCharacter::OnMontageEnded);
				AnimInstance->Montage_SetEndDelegate(EndDelegate, HitReactMontage);
			}
		}
	}

	CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);
	OnHealthChanged.Broadcast(CurrentHealth, MaxHealth);

	if (CurrentHealth <= 0.0f)
	{
		Die();
	}

	return ActualDamage;
}

void AFighterCharacter::Die()
{
	SetCombatState(EFighterCombatState::Dead);
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	if (DeathMontage)
	{
		UAnimInstance* AnimInstance = GetMesh() ? GetMesh()->GetAnimInstance() : nullptr;
		if (AnimInstance)
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}

	if (bIsLockedOn)
	{
		ToggleLockOn();
	}
}

void AFighterCharacter::ResetCombo()
{
	ComboIndex = 0;
	if (CombatState == EFighterCombatState::Attacking)
	{
		SetCombatState(EFighterCombatState::Idle);
	}
}

void AFighterCharacter::OnMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (CombatState != EFighterCombatState::Dead && CombatState != EFighterCombatState::Blocking)
	{
		SetCombatState(EFighterCombatState::Idle);
	}
}

void AFighterCharacter::ToggleBulletTime()
{
	if (TimeDilationComp)
	{
		TimeDilationComp->ToggleBulletTime();
	}
}

void AFighterCharacter::SwitchWeapon(EWeaponSlot Slot)
{
	if (InventoryComp)
	{
		InventoryComp->SwitchToSlot(Slot);
	}
}

void AFighterCharacter::NextWeapon()
{
	if (InventoryComp)
	{
		InventoryComp->NextWeapon();
	}
}

void AFighterCharacter::PreviousWeapon()
{
	if (InventoryComp)
	{
		InventoryComp->PreviousWeapon();
	}
}

void AFighterCharacter::ReloadWeapon()
{
	if (InventoryComp)
	{
		InventoryComp->Reload();
	}
}

void AFighterCharacter::DoubleJump()
{
	if (ParkourComp)
	{
		ParkourComp->TryDoubleJump();
	}
}

void AFighterCharacter::AirDash()
{
	if (ParkourComp)
	{
		FVector InputDir = GetCharacterMovement() ? GetCharacterMovement()->GetLastInputVector() : FVector::ZeroVector;
		ParkourComp->TryAirDash(InputDir);
	}
}

void AFighterCharacter::StartSlide()
{
	if (ParkourComp)
	{
		ParkourComp->StartSlide();
	}
}

void AFighterCharacter::StopSlide()
{
	if (ParkourComp)
	{
		ParkourComp->StopSlide();
	}
}
