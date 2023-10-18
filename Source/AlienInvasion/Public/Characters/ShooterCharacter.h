// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "Items/Weapons/AmmoType.h"
#include "ShooterCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UInputMappingContext;
class UInputAction;
class USoundBase;
class UParticleSystem;
class UAnimMontage;
class AItem;
class AWeapon;

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_Firing UMETA(DisplayName = "Firing"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class ALIENINVASION_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AShooterCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void GetPickupItem(AItem* Item);

protected:
	virtual void BeginPlay() override;

	/** Input actions */
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* ShooterCharacterMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* FireButtonAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AimButtonAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ActionButtonAction;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* ReloadButtonAction;

	/** Input callbacks */
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Jump();
	void FireButtonPressed(const FInputActionValue& Value);
	void AimButton(const FInputActionValue& Value);
	void ActionButton(const FInputActionValue& Value);
	void ReloadButtonPressed(const FInputActionValue& Value);

	void FireWeapon();
	void ReloadWeapon();
	void PlayFireSound();
	void SendBullet();
	void PlayGunfireMontage();
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation);
	bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);
	void CameraInterpZoom(float DeltaTime);
	void SetLookRates();
	void CalculateCrosshairSpread(float DeltaTime);
	void TraceForItems();

	AWeapon* SpawnDefaultWeapon();
	void EquipWeapon(AWeapon* WeaponToEquip);
	void DropWeapon();
	void SwapWeapon(AWeapon* WeaponToSwap);
	void InitializeAmmoMap();
	bool WeaponHasAmmo();
	bool CarryingAmmo();

	void StartCrosshairBulletFire();

	UFUNCTION()
	void FinishCrosshairBulletFire();

	UFUNCTION(BlueprintCallable)
	void FinishReloading();

	UFUNCTION(BlueprintCallable)
	void GrabMagazine();

	UFUNCTION(BlueprintCallable)
	void ReleaseMagazine();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	/** Randomized gunshot sound */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundBase* FireSound;

	/** Flash spawned at BarrelSocket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* MuzzleFlash;

	/** Particles spawned upon bullet impact */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;

	/** Smoke trail for bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	/** True when aiming */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bAiming = false;

	/** Default camera field of view value */
	float CameraDefaultFOV = 0.f;

	/** Field of view value when zoomed in */
	float CameraZoomedFOV = 45.f;

	/** Current field of view this frame */
	float CameraCurrentFOV = 0.f;

	/** Interp speed for zooming when aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed = 20.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate = 45.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate = 45.f;

	/** Turn rate while not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float HipTurnRate = 1.0f;

	/** Look up rate when not aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float HipLookUpRate = 1.0f;

	/** Turn rate when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float AimingTurnRate = 0.2f;

	/** Look up rate when aiming */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float AimingLookUpRate = 0.2f;

	/** Determines the spread of the crosshair based on multiple factors */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairSpreadMultiplier = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairVelocityFactor = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairInAirFactor = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairAimFactor = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Crosshairs, meta = (AllowPrivateAccess = "true"))
	float CrosshairShootingFactor = 0.f;

	/** Left mouse button or right console trigger pressed */
	bool bFireButtonPressed;

	float ShootTimeDuration = 0.05f;
	bool bFiringBullet = false;
	FTimerHandle CrosshairShootTimer;

	/** Check to see if character can jump */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bCanJump = true;

	/** True if we should trace every frame for items */
	bool bShouldTraceForItems = false;

	/** Number of overlapped AItems */
	int8 OverlappedItemCount;

	/** The AItem we traced last frame */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItemLastFrame;

	/** The item currently hit by our trace in TraceForItems */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	/** Currently equipped weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class AWeapon* EquippedWeapon;

	/** Set this in blueprints for the default weapon class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/** Distance outward from the camera for the interp destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpDistance = 200.f;

	/** Distance upward from the camera for the interp destination */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	float CameraInterpElevation = 65.f;

	/** Map to keep track of ammo of different types */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	/** Starting amount of 9mm ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 Starting9mmAmmo = 85;

	/** Starting amount of AR ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo = 120;

	/** Combat State, can only fire or reload if unoccupied */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState = ECombatState::ECS_Unoccupied;

	/** Transform of the clip when we first grab the clip during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;

	/** Scene component to attach to the character's hand during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

	/** Animation Montages */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* HipFireMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

public:
	FORCEINLINE UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	FORCEINLINE bool GetAiming() const { return bAiming; }

	UFUNCTION(BlueprintCallable)
	float GetCrosshairSpreadMultiplier() const;

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }
	FORCEINLINE ECombatState GetCombatState() const { return CombatState; }

	/** Add/subtracts to/from OverlappedItemCount and updates bShouldTraceForItems */
	void UpdateOverlappedItemCount(int8 Amount);
	FVector GetCameraInterpLocation();
};
