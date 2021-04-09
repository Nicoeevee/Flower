#include "Flow/Nodes/FlowNode_OnInteractionUsed.h"
#include "Components/InteractionComponent.h"

#include "FlowComponent.h"

UFlowNode_OnInteractionUsed::UFlowNode_OnInteractionUsed(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

void UFlowNode_OnInteractionUsed::ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	if (!ObservedInteractions.Contains(Actor))
	{
		TArray<UInteractionComponent*> FoundInteractions;
		Actor->GetComponents<UInteractionComponent>(FoundInteractions);

		if (FoundInteractions.Num() > 0)
		{
			RegisteredActors.Emplace(Actor, Component);

			ObservedInteractions.Emplace(Actor, FoundInteractions[0]);
			FoundInteractions[0]->OnUsed.AddDynamic(this, &UFlowNode_OnInteractionUsed::OnEventReceived);
		}
	}
}

void UFlowNode_OnInteractionUsed::ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component)
{
	ensureAlways(ObservedInteractions.Contains(Component->GetOwner()));
	TWeakObjectPtr<UInteractionComponent> InteractionComponent = ObservedInteractions[Component->GetOwner()];

	InteractionComponent->OnUsed.RemoveAll(this);
}

FString UFlowNode_OnInteractionUsed::GetInteractionProgress() const
{
	if (ObservedInteractions.Num())
	{
		return GetProgressAsString(SuccessCount) + TEXT(" / ") + GetProgressAsString(SuccessLimit);
	}
	return FString();
}

FString UFlowNode_OnInteractionUsed::GetStatusString() const
{
	return GetInteractionProgress();
}

void UFlowNode_OnInteractionUsed::Cleanup()
{
	Super::Cleanup();

	for (const TPair<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>>& Interaction : ObservedInteractions)
	{
		Interaction.Value->OnUsed.RemoveAll(this);
	}
	ObservedInteractions.Empty();
}
