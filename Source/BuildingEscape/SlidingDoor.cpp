// Kaede 2020


#include "SlidingDoor.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"

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
	FVector origin;
	
	StartLocation = GetOwner()->GetActorLocation();
	GetOwner()->GetActorBounds(false, origin, boxExtent, false);
	
	UE_LOG(LogTemp, Warning, TEXT("Origin are: %f, %f, %f"), origin.X, origin.Y, origin.Z);
	UE_LOG(LogTemp, Warning, TEXT("boxExtent are: %f, %f, %f"), boxExtent.X, boxExtent.Y, boxExtent.Z);
	TargetLocation.X = StartLocation.X;
	TargetLocation.Y = bIsOpen ? StartLocation.Y + boxExtent.Y * 2 : StartLocation.Y - boxExtent.Y * 2;
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
	TargetLocation.Y = bIsOpen ? Location.Y + boxExtent.Y * 2 : Location.Y - boxExtent.Y * 2;
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

