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
	// ...
}


// Called every frame
void USlidingDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Location = GetOwner()->GetActorLocation();
	if (DoorPressurePlate && Location.Y != OpenLocation.Y && DoorPressurePlate->IsOverlappingActor(ActorThatOpen)) {
		SlideDoor(DeltaTime, true);
	} else if (Location.Y != CloseLocation.Y) {
		SlideDoor(DeltaTime, false);
	}
	// ...
}

void USlidingDoor::SlideDoor(float& DeltaTime, bool bIsDoorOpening)
{
	FVector Location = GetOwner()->GetActorLocation();
	FVector TargetLocation = bIsDoorOpening ? OpenLocation : CloseLocation;
	float InterpolatedY = FMath::FInterpConstantTo(Location.Y, TargetLocation.Y, DeltaTime,  45);
	Location.Y = InterpolatedY;
	GetOwner()->SetActorLocation(Location);
	UE_LOG(LogTemp, Warning, TEXT("actor Y is: %f"), Location.Y);
}