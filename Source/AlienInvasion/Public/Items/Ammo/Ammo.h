// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Items/Weapons/AmmoType.h"
#include "Ammo.generated.h"

/**
 * 
 */
UCLASS()
class ALIENINVASION_API AAmmo : public AItem
{
	GENERATED_BODY()

public:
	AAmmo();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	virtual void SetItemProperties(EItemState State) override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* AmmoMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	EAmmoType AmmoType = EAmmoType::EAT_9mm;

public:
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
};
