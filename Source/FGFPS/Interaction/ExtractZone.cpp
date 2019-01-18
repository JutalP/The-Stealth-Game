#include "Interaction/ExtractZone.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Player/FGPlayerCharacter.h"
#include "Image.h"


AExtractZone::AExtractZone()
{
	PrimaryActorTick.bCanEverTick = true;

	OverlapComp = CreateDefaultSubobject<UBoxComponent>("OverlapComp");
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	RootComponent = OverlapComp;

	OverlapComp->SetHiddenInGame(true);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AExtractZone::HandleOverlap);

	DecalComp = CreateDefaultSubobject<UDecalComponent>("DecalComp");
	DecalComp->DecalSize = FVector(200,200,200);
	DecalComp->SetupAttachment(RootComponent);
}

void AExtractZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Error, TEXT("Overlapped with ExtractionZone"));

	AFGPlayerCharacter* MyPlayer = Cast<AFGPlayerCharacter>(OtherActor);

	if (MyPlayer && MyPlayer->bIsCarryingKey == true)
	{
		CompleteMission(MyPlayer, true);
	MyPlayer->MyExtractionZone = this;
	}
}

void AExtractZone::CompleteMission(AActor* OtherActor, bool bMissionSuccess)
{

	if (OtherActor)
	{
		OtherActor->DisableInput(nullptr);
	}

	OnCompleteMission(OtherActor, bMissionSuccess);
}
