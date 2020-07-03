// Kaede 2020


#include "OpenDoor.h"
#include "GameFramework/Actor.h"
// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	AActor* Owner = GetOwner();
	FVector Location = Owner->GetActorLocation();
	OpenLocation.X = Location.X;
	OpenLocation.Y = Location.Y - 105;
	OpenLocation.Z = Location.Z;
	// ...
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!bIsOpen) {
		FVector Location = GetOwner()->GetActorLocation();
		float InterpolatedY = FMath::Lerp(Location.Y, OpenLocation.Y, DeltaTime + 0.02F);
		FVector SmoothTransition = Location;
		SmoothTransition.Y = InterpolatedY;
		GetOwner()->SetActorLocation(SmoothTransition);
		if (SmoothTransition.Y == OpenLocation.Y) {	
			bIsOpen = true;
		}
	}
	// ...
}

