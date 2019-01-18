#include "Interaction/KeyPickup.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Components/FGInteractableComponent.h"
#include "Player/FGPlayerCharacter.h"
#include "Components/FGInteractorComponent.h"


AKeyPickup::AKeyPickup()
{

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("MeshComp");

	RootComponent = MeshComp;

}

void AKeyPickup::OnInteractionComplete(UFGInteractorComponent* Interactor)
{
	AFGPlayerCharacter* MyPlayer = Cast<AFGPlayerCharacter>(Interactor->GetOwner());

	
	if (MyPlayer)
	{
		MyPlayer->MyExtractionZone = MyExtractZone;
		MyPlayer->bIsCarryingKey = true;
	}
}
