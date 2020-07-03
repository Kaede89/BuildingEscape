// Kaede 2020


#include "SlidingDoor.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"

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
	AActor* Owner = GetOwner();
	StartLocation = Owner->GetActorLocation();
	TargetLocation.Y = bIsOpen ? StartLocation.Y + 105 : StartLocation.Y - 105;
	// ...
}


// Called every frame
void USlidingDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	FVector Location = GetOwner()->GetActorLocation();
	if (Location.Y != TargetLocation.Y) {
		SlideDoor(DeltaTime);
	}
	// ...
}

void USlidingDoor::RecalculateTargetPosition()
{
	UE_LOG(LogTemp, Error, TEXT("timer expired: is open? %s"), bIsOpen ? TEXT("true") : TEXT("false"));
	FVector Location = GetOwner()->GetActorLocation();
	TargetLocation.Y = bIsOpen ? Location.Y + 105 : Location.Y - 105;
}

void USlidingDoor::SlideDoor(float& DeltaTime)
{
	FVector Location = GetOwner()->GetActorLocation();
	float InterpolatedY = FMath::FInterpConstantTo(Location.Y, TargetLocation.Y, DeltaTime,  45);
	FVector Transition = Location;
	Transition.Y = InterpolatedY;
	GetOwner()->SetActorLocation(Transition);
	UE_LOG(LogTemp, Warning, TEXT("actor Y is: %f"), Transition.Y);
	if (Transition.Y == TargetLocation.Y) {
		bIsOpen = !bIsOpen;
		FTimerHandle UnusedHandle;
		UE_LOG(LogTemp, Error, TEXT("Setting up timer!"));
		GetOwner()->GetWorldTimerManager().SetTimer(UnusedHandle, this, &USlidingDoor::RecalculateTargetPosition, 2.0f, false, 2.0f);
	}
}

