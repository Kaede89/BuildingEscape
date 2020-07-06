// Kaede 2020


#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandle();
	SetupInputComponent();
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
	{
		UE_LOG(LogTemp, Error, TEXT("no physics handle component found attached to: %s"), *GetOwner()->GetName());
	}
}

void UGrabber::SetupInputComponent()
{
	InputController = GetOwner()->FindComponentByClass<UInputComponent>();

	InputController->BindAction
	(
		TEXT("Grab"),
		EInputEvent::IE_Pressed,
		this,
		&UGrabber::Grab
	);

	InputController->BindAction
	(
		TEXT("Grab"),
		EInputEvent::IE_Released,
		this,
		&UGrabber::Release
	);	
}

void UGrabber::Grab()
{
	FHitResult Hit = GetFirstPhysicsBodyInReach();

	UPrimitiveComponent* ComponentToGrab = Hit.GetComponent();

	if (Hit.GetActor())
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, GetPlayerReach());
	}
}

void UGrabber::Release()
{
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(GetPlayerReach());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetPlayerLocation(),
		GetPlayerReach(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParams
	);

	return Hit;
}

FVector UGrabber::GetPlayerReach() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);
	
	return PlayerViewLocation + PlayerViewRotation.Vector() * Reach;
}

FVector UGrabber::GetPlayerLocation() const
{
	FVector PlayerViewLocation;
	FRotator PlayerViewRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(PlayerViewLocation, PlayerViewRotation);

	return PlayerViewLocation;
}

