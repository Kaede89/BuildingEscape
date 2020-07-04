// Kaede 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "SlidingDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API USlidingDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USlidingDoor();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere)
	bool bIsOpen = false;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* DoorPressurePlate;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen;
	
	FVector OpenLocation;
	FVector CloseLocation;
	
	
	void SlideDoor(float& DeltaTime, bool bIsDoorOpening);
};
