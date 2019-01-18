#pragma once 

#include "GameFramework/Actor.h"
#include "ExtractZone.h"
#include "KeyPickup.generated.h"

UCLASS()
class FGFPS_API AKeyPickup : public AActor
{
	GENERATED_BODY()

public:

	AKeyPickup();

	UFUNCTION(BlueprintCallable, Category = Interaction)
		void OnInteractionComplete(class UFGInteractorComponent* Interactor);

	UPROPERTY(EditAnywhere)
		class AExtractZone* MyExtractZone;

protected:
	UPROPERTY(VisibleAnywhere, Category = Mesh)
	    class UStaticMeshComponent* MeshComp;

	class UFGInteractableComponent* MyInteraction;


};
