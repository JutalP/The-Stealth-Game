#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Widget.h"
#include "UserWidget.h"
#include "ExtractZone.generated.h"

UCLASS()
class FGFPS_API AExtractZone : public AActor
{
	GENERATED_BODY()

public:

	AExtractZone();

		void CompleteMission(AActor* OtherActor, bool bMissionSuccess);

protected:

	UPROPERTY(VisibleAnywhere, Category = Components)
		class UBoxComponent* OverlapComp;

	UPROPERTY(VisibleAnywhere, Category = Components)
		class UDecalComponent* DecalComp;

	UFUNCTION()
		void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

		UFUNCTION(BlueprintImplementableEvent, Category = Event)
			void OnCompleteMission(AActor * OtherActor, bool bMissionSuccess);
};