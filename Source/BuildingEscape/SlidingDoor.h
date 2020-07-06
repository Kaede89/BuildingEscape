// Kaede 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
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
	float TotalMassOfActors() const;

private:
	UPROPERTY(EditAnywhere)
	bool bIsOpen = false;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* DoorPressurePlate = nullptr;

	UPROPERTY(EditAnywhere)
	AActor* ActorThatOpen = nullptr;
	
	UPROPERTY(EditAnywhere)
	float DoorCloseDelay = 1.f;

	UPROPERTY(EditAnywhere)
	float DoorSlideSpeed = 85.f;

	UPROPERTY(EditAnywhere)
	float MassToOpenDoor = 0.f;
	
	float DoorLastOpened = 0.f;
	
	FVector OpenLocation;
	FVector CloseLocation;
	TMap<FString, UAudioComponent*> AudioComponentsMap;
	bool bOpenDoorSound = false;
	bool bCloseDoorSound = false;

	void SlideDoor(float& DeltaTime, bool bIsDoorOpening);
	void FindAudioComponents();
	void PlayAudio(bool bIsDoorOpening);
	void ResetAudio();
};
