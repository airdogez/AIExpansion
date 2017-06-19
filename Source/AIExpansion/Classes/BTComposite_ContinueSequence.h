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

UCLASS()
class AIEXPANSION_API UBTComposite_ContinueSequence : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const ;

	UPROPERTY(EditAnywhere)
	bool ContinueFromFailedChild;
	
	UPROPERTY(EditAnywhere)
	uint8 MaxTries;

#if WITH_EDITOR
	virtual bool CanAbortLowerPriority() const override;
	virtual FName GetNodeIconName() const override;
#endif

	/** size of instance memory */
	virtual uint16 GetInstanceMemorySize() const override;
};

