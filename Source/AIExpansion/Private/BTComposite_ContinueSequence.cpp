
#include "BTComposite_ContinueSequence.h"

#include "BehaviorTree/BTCompositeNode.h" 

UBTComposite_ContinueSequence::UBTComposite_ContinueSequence(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	NodeName = "Continue Sequence";

	OnNextChild.BindUObject(this, &UBTComposite_ContinueSequence::GetNextChildHandler);
}

int32 UBTComposite_ContinueSequence::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
	FBTContinueSequenceMemory* NodeMemory = GetNodeMemory<FBTContinueSequenceMemory>(SearchData);
	// failure = quit
	int32 NextChildIdx = BTSpecialChild::ReturnToParent;

	if (NodeMemory->SequenceOverride != BTSpecialChild::NotInitialized) {
		NextChildIdx = NodeMemory->SequenceOverride;
		NodeMemory->SequenceOverride = BTSpecialChild::NotInitialized;
	}
	else {
		if (PrevChild == BTSpecialChild::NotInitialized)
		{
			// newly activated: start from last in memory
			NextChildIdx = 0; //ContinueChild
		}
		else if (LastResult == EBTNodeResult::Succeeded && (PrevChild + 1) < GetChildrenNum())
		{
			// success = choose next child
			NextChildIdx = PrevChild + 1;
		}

		if (LastResult == EBTNodeResult::Failed && NodeMemory->Tries < MaxTries) {
			NodeMemory->SequenceOverride = (ContinueFromFailedChild ? PrevChild : PrevChild + 1);
			NodeMemory->Tries += 1;
		}
		else {
			NodeMemory->SequenceOverride = BTSpecialChild::NotInitialized;
			NodeMemory->Tries = 0;
		}
	}

	return NextChildIdx;
}

#if WITH_EDITOR

bool UBTComposite_ContinueSequence::CanAbortLowerPriority() const
{
	// don't allow aborting lower priorities, as it breaks sequence order and doesn't makes sense
	return false;
}

FName UBTComposite_ContinueSequence::GetNodeIconName() const
{
	return FName("BTEditor.Graph.BTNode.Composite.Sequence.Icon");
}

#endif

uint16 UBTComposite_ContinueSequence::GetInstanceMemorySize() const
{
	return sizeof(FBTContinueSequenceMemory);
}
