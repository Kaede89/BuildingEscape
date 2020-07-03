// Kaede 2020

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	
	FVector TargetLocation;
	FVector StartLocation;
	
	void SlideDoor(float& DeltaTime);
	void RecalculateTargetPosition();
};
