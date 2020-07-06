// Kaede 2020


#include "SlidingDoor.h"



// Sets default values for this component's properties
USlidingDoor::USlidingDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USlidingDoor::BeginPlay()
{
	Super::BeginPlay();

	if(!DoorPressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has the sliding door component, but no pressure plate found"), *GetOwner()->GetName());
	}

	FVector origin, boxExtent;

	GetOwner()->GetActorBounds(false, origin, boxExtent, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Origin are: %f, %f, %f"), origin.X, origin.Y, origin.Z);
	UE_LOG(LogTemp, Warning, TEXT("boxExtent are: %f, %f, %f"), boxExtent.X, boxExtent.Y, boxExtent.Z);

	if (bIsOpen) {
		OpenLocation = origin;
		CloseLocation = OpenLocation;
		CloseLocation.Y = OpenLocation.Y + boxExtent.Y * 2;
	} else {
		CloseLocation = origin;
		OpenLocation = CloseLocation;
		OpenLocation.Y = CloseLocation.Y - boxExtent.Y * 2;
	}

	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();

	FindAudioComponents();
	// ...
}


// Called every frame
void USlidingDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Location = GetOwner()->GetActorLocation();
	if (DoorPressurePlate && TotalMassOfActors() > MassToOpenDoor)
	{
		if (Location.Y != OpenLocation.Y)
		{
			SlideDoor(DeltaTime, true);
		}

		if (GetOwner()->GetActorLocation().Y == OpenLocation.Y)
		{
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
	}
	else if (Location.Y != CloseLocation.Y)
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpened > DoorCloseDelay)
		{
			SlideDoor(DeltaTime, false);
		}
	}
	// ...
}

void USlidingDoor::SlideDoor(float& DeltaTime, bool bIsDoorOpening)
{
	FVector Location = GetOwner()->GetActorLocation();
	FVector TargetLocation = bIsDoorOpening ? OpenLocation : CloseLocation;
	float InterpolatedY = FMath::FInterpConstantTo(Location.Y, TargetLocation.Y, DeltaTime,  DoorSlideSpeed);
	Location.Y = InterpolatedY;
	GetOwner()->SetActorLocation(Location);
	PlayAudio(bIsDoorOpening);
	if (Location.Y == TargetLocation.Y)
	{
		ResetAudio();
	}
	//UE_LOG(LogTemp, Warning, TEXT("actor Y is: %f"), Location.Y);
}

float USlidingDoor::TotalMassOfActors() const
{

	TArray<AActor*> OverlappingActors;
	float TotalMass = 0.f;
	if (DoorPressurePlate)
	{
		DoorPressurePlate->GetOverlappingActors(OverlappingActors);

		for (AActor* Actor : OverlappingActors)
		{
			float AMass =  Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
			UE_LOG(LogTemp, Warning, TEXT("mass: %f for %s"), AMass, *Actor->GetName());
			TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}

	}

	// UE_LOG(LogTemp, Warning, TEXT("total mass is: %f"), TotalMass);
	return TotalMass;
}

void USlidingDoor::FindAudioComponents()
{
	TArray<UAudioComponent*> AudioComponents;
	GetOwner()->GetComponents(AudioComponents);

	if (AudioComponents.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("No Audio component attached to %s"), *GetOwner()->GetName());
	}

	for (auto Audio : AudioComponents)
	{
		AudioComponentsMap.Add(Audio->GetName(), Audio);
	}
}

void USlidingDoor::PlayAudio(bool bIsDoorOpening)
{
	const FString AudioComponentName = bIsDoorOpening ? TEXT("OpenDoorAudio") : TEXT("CloseDoorAudio");

	if (!AudioComponentsMap.Contains(AudioComponentName))
	{
		UE_LOG(LogTemp, Warning, TEXT("No Audio component attached named %s"), *AudioComponentName);
		return;
	}

	const bool SoundToCheck = bIsDoorOpening ? bOpenDoorSound : bCloseDoorSound;

	if (!SoundToCheck)
	{
		AudioComponentsMap[AudioComponentName]->Play();

		if (bIsDoorOpening)
		{
			bOpenDoorSound = true;
		}
		else
		{
			bCloseDoorSound = true;
		}
	}
}

void USlidingDoor::ResetAudio()
{
	bOpenDoorSound = false;
	bCloseDoorSound = false;
}