#pragma once


#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTComposite_ContinueSequence.generated.h"

class UBTCompositeNode;

struct FBTContinueSequenceMemory : FBTCompositeMemory
{
	/** child override for next selection */
	uint8 SequenceOverride;
	uint8 Tries;
};


/**
 * Continue Sequence composite node.
 * Continue Sequence Nodes execute their children from left to right, and will stop executing its children when one of their children fails.
 * If a child fails, then the Sequence fails. If all the Sequence's children succeed, then the Sequence succeeds.
 * Additionaly, When the Sequence fails, the Child index will be saved and the next execution will continue from that position or the next child
 * depending on the settings, for an amount defined in MaxTries.
 */
UCLASS()
class AIEXPANSION_API UBTComposite_ContinueSequence : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const ;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool ContinueFromLastChild;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	uint8 MaxTries;

#if WITH_EDITOR
	virtual bool CanAbortLowerPriority() const override;
	virtual FName GetNodeIconName() const override;
#endif

	/** size of instance memory */
	virtual uint16 GetInstanceMemorySize() const override;
};

