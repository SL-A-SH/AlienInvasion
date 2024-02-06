// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Health.generated.h"

class USphereComponent;
class USoundBase;

UCLASS()
class ALIENINVASION_API AHealth : public AActor
{
	GENERATED_BODY()
	
public:	
	AHealth();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties")
	UStaticMeshComponent* HealthSM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	USphereComponent* HealthSphere;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	USoundBase* PickupSound;

private:
	float HealingAmount = 20.f;
		
};
