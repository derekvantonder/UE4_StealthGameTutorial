// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSObjectiveActor.generated.h"

class UParticleSystem;
class USphereComponent;
class UStaticMeshComponent;

UCLASS()
class AFPSObjectiveActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFPSObjectiveActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USphereComponent* SphereComp;

    UPROPERTY(EditDefaultsOnly, Category = "Components")
    UParticleSystem* PickupFX;

    void PlayEffects();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    // Event when this actor overlaps another actor, for example a player walking into a trigger.
    virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
};
