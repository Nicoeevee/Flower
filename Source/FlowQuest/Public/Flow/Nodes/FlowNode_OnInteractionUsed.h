#pragma once

#include "Nodes/World/FlowNode_ComponentObserver.h"
#include "FlowNode_OnInteractionUsed.generated.h"

class UFlowComponent;
class UInteractionComponent;

/**
 * On Interaction Used
 */
UCLASS(NotBlueprintable, meta = (DisplayName = "On Interaction Used"))
class FLOWQUEST_API UFlowNode_OnInteractionUsed : public UFlowNode_ComponentObserver
{
	GENERATED_UCLASS_BODY()

private:
	TMap<TWeakObjectPtr<AActor>, TWeakObjectPtr<UInteractionComponent>> ObservedInteractions;

protected:
	virtual void ObserveActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;
	virtual void ForgetActor(TWeakObjectPtr<AActor> Actor, TWeakObjectPtr<UFlowComponent> Component) override;

public:
	FString GetInteractionProgress() const;

#if WITH_EDITOR
	virtual FString GetStatusString() const override;
#endif

protected:
	virtual void Cleanup() override;
};
