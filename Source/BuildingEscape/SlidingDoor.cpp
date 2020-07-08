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

	FindDoorMeshComponent();
	SetDoorInitialPosition();
	BindCollisions();
	FindAudioComponents();
	ActorThatOpen = GetWorld()->GetFirstPlayerController()->GetPawn();
}


// Called every frame
void USlidingDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector DoorLocation = DoorStaticMeshComponent->GetRelativeLocation();
	if ((bIsUnlocked && bIsDoorOpening) || (DoorPressurePlate && TotalMassOfActors() > MassToOpenDoor))
	{
		if (DoorLocation.Y < DoorOpenRelativePosition)
		{
			SlideDoor(DeltaTime);
		}

		if (DoorLocation.Y == DoorOpenRelativePosition)
		{
			DoorLastOpened = GetWorld()->GetTimeSeconds();
		}
	}
	else if (bIsDoorClosing && DoorLocation.Y > DoorClosedRelativeLocation && !bPawnColliding)
	{
		SlideDoor(DeltaTime);
	}
	// ...
}

void USlidingDoor::SlideDoor(float& DeltaTime)
{
	FVector Location = DoorStaticMeshComponent->GetRelativeLocation();
	float TargetRelativeLocation = bIsDoorOpening ? DoorOpenRelativePosition : DoorClosedRelativeLocation;
	float InterpolatedY = FMath::FInterpConstantTo(Location.Y, TargetRelativeLocation, DeltaTime,  DoorSlideSpeed);
	Location.Y = InterpolatedY;
	DoorStaticMeshComponent->SetRelativeLocation(Location);
	PlayAudio();
	if (Location.Y == TargetRelativeLocation)
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

void USlidingDoor::PlayAudio()
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

void USlidingDoor::OnOverlapBegin(class UPrimitiveComponent* OverlapComponent, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("overlapped!"));

	if (bIsUnlocked)
	{
		bIsDoorOpening = true;
		bIsDoorClosing = false;
	}
	else if (AudioComponentsMap.Contains(TEXT("AccessDeniedAudio")) && !bAccessDeniedSound)
	{
		AudioComponentsMap[TEXT("AccessDeniedAudio")]->Play();
		bAccessDeniedSound = true;
	}

	bPawnColliding = true;
}

void USlidingDoor::onOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{	
	UE_LOG(LogTemp, Warning, TEXT("overlap ended!"));
	
	if (bIsUnlocked)
	{
		bIsDoorOpening = false;
		bIsDoorClosing = true;
	}

	bAccessDeniedSound = false;
	bPawnColliding = false;
}

void USlidingDoor::FindDoorMeshComponent()
{
	UStaticMeshComponent* StaticMeshComponent = GetOwner()->FindComponentByClass<UStaticMeshComponent>();
	TArray<UStaticMeshComponent*> StaticMeshes;
	GetOwner()->GetComponents(StaticMeshes);

	for (auto StaticMesh : StaticMeshes)
	{
		if (StaticMesh->GetName().Equals(TEXT("Door")))
		{
			DoorStaticMeshComponent = StaticMesh;
			break;
		}
	}

	if (!DoorStaticMeshComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no valid static mesh component Door"), *GetOwner()->GetName());
	}

}

void USlidingDoor::BindCollisions()
{
	UBoxComponent* CollisionBoxComponent = GetOwner()->FindComponentByClass<UBoxComponent>();

	if (!CollisionBoxComponent)
	{
		UE_LOG(LogTemp, Error, TEXT("%s has no valid Collision Box Component"), *GetOwner()->GetName());
		return;
	}

	CollisionBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &USlidingDoor::OnOverlapBegin);
	CollisionBoxComponent->OnComponentEndOverlap.AddDynamic(this, &USlidingDoor::onOverlapEnd);
}

void USlidingDoor::SetDoorInitialPosition()
{
	FVector InitialPosition = DoorStaticMeshComponent->GetRelativeLocation();
	InitialPosition.Y += DoorStartingPosition;
	DoorStaticMeshComponent->SetRelativeLocation(InitialPosition);
}