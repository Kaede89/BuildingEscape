// Kaede 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/PrimitiveComponent.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
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
	bool bIsUnlocked = false;

	bool bPawnColliding = false;

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

	UPROPERTY(EditAnywhere)
	float DoorOpenRelativePosition = 120.f;

	UPROPERTY(EditAnywhere)
	float DoorStartingPosition = 0.f;

	const float DoorClosedRelativeLocation = 0.f;
	bool bIsDoorOpening = false;
	bool bIsDoorClosing = false;

	UStaticMeshComponent* DoorStaticMeshComponent = nullptr;
	
	FVector OpenLocation;
	FVector CloseLocation;
	TMap<FString, UAudioComponent*> AudioComponentsMap;
	bool bOpenDoorSound = false;
	bool bCloseDoorSound = false;
	bool bAccessDeniedSound = false;

	void SlideDoor(float& DeltaTime);
	void FindAudioComponents();
	void FindDoorMeshComponent();
	void BindCollisions();
	void SetDoorInitialPosition();
	void PlayAudio();
	void ResetAudio();
	UFUNCTION()
 	void OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void onOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
