
#include "BTComposite_ContinueSelector.h"

#include "BehaviorTree/BTCompositeNode.h" 

UBTComposite_ContinueSelector::UBTComposite_ContinueSelector(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Continue Selector";

	OnNextChild.BindUObject(this, &UBTComposite_ContinueSelector::GetNextChildHandler);
}

int32 UBTComposite_ContinueSelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	FBTContinueSelectorMemory* NodeMemory = GetNodeMemory<FBTContinueSelectorMemory>(SearchData);
	// failure = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (NodeMemory->SelectorOverride != BTSpecialChild::NotInitialized) {
		NextChildIdx = NodeMemory->SelectorOverride;
		NodeMemory->SelectorOverride = BTSpecialChild::NotInitialized;
	}
	else {

		if (PrevChild == BTSpecialChild::NotInitialized)
		{
			// newly activated: start from first
			NextChildIdx = 0;
		}
		else if (LastResult == EBTNodeResult::Failed && (PrevChild + 1) < GetChildrenNum())
		{
			// failed = choose next child
			NextChildIdx = PrevChild + 1;
		}

		if (LastResult == EBTNodeResult::Succeeded && NodeMemory->Tries < MaxTries) {
			NodeMemory->SelectorOverride = (ContinueFromLastChild ? PrevChild : PrevChild + 1);
			NodeMemory->Tries += 1;
		}
		else {
			NodeMemory->SelectorOverride = BTSpecialChild::NotInitialized;
			NodeMemory->Tries = 0;
		}
	}

	return NextChildIdx;
}

#if WITH_EDITOR

/*
bool UBTComposite_ContinueSelector::CanAbortLowerPriority() const
{
	// don't allow aborting lower priorities, as it breaks sequence order and doesn't makes sense
	return false;
} 
*/

FName UBTComposite_ContinueSelector::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Selector.Icon");
}

#endif

uint16 UBTComposite_ContinueSelector::GetInstanceMemorySize() const
{
	return sizeof(FBTContinueSelectorMemory);
}
