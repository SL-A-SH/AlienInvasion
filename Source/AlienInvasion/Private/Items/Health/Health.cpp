// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Health/Health.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/ShooterCharacter.h"

AHealth::AHealth()
{
	PrimaryActorTick.bCanEverTick = true;

	HealthSM = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HealthSM"));
	SetRootComponent(HealthSM);

	HealthSphere = CreateDefaultSubobject<USphereComponent>(TEXT("PickupSphere"));
	HealthSphere->SetupAttachment(GetRootComponent());
}

void AHealth::BeginPlay()
{
	Super::BeginPlay();

	HealthSphere->OnComponentBeginOverlap.AddDynamic(this, &AHealth::OnSphereOverlap);
}

void AHealth::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealth::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"))
	if (OtherActor)
	{
		AShooterCharacter* Character = Cast<AShooterCharacter>(OtherActor);
		if (Character)
		{
			if (PickupSound)
			{
				UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());
			}

			float Health = Character->GetHealth();
			float HealthToAdd = Health + HealingAmount;

			if (HealthToAdd > Character->GetMaxHealth())
			{
				HealthToAdd = Character->GetMaxHealth();
			}

			Character->SetHealth(HealthToAdd);

			Destroy();
		}
	}
}

