#pragma once


#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTComposite_ContinueSelector.generated.h"

class UBTCompositeNode;

struct FBTContinueSelectorMemory : FBTCompositeMemory
{
	/** child override for next selection */
	uint8 SelectorOverride;
	uint8 Tries;
};

/** 
 * Continue Selector composite node.
 * Continue Selector Nodes execute their children from left to right, and will stop executing its children when one of their children succeeds.
 * If a Selector's child succeeds, the Selector succeeds. If all the Selector's children fail, the Selector fails.
 * Additionaly, when the Selection succeeds, the last child index will be saved and next execution will continue from that child or the next one.
 */
UCLASS()
class AIEXPANSION_API UBTComposite_ContinueSelector : public UBTCompositeNode
{
	GENERATED_UCLASS_BODY()

	int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const ;

	UPROPERTY(EditAnywhere, Category = "Settings")
	bool ContinueFromLastChild;
	
	UPROPERTY(EditAnywhere, Category = "Settings")
	uint8 MaxTries;

#if WITH_EDITOR
	//virtual bool CanAbortLowerPriority() const override;
	virtual FName GetNodeIconName() const override;
#endif

	/** size of instance memory */
	virtual uint16 GetInstanceMemorySize() const override;
};

