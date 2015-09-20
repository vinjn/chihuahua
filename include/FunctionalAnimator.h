// Copyright (C) 2015-2015 Vinjn Zhang
// This file is part of the "UGraphics".

#pragma once

#include "ISceneNodeAnimator.h"
#include "ISceneNode.h"
#include <functional>

namespace irr
{
namespace scene
{

/*
FunctionalAnimator::addFunctionToNode(node, [](ISceneNode* node, u32 timeMs){
    printf("%d\n", node->getID());
});
*/
struct FunctionalAnimator : public ISceneNodeAnimator
{
	typedef std::function<void(ISceneNode*, u32)> Function;

	static void addFunctionToNode(ISceneNode* node, Function action)
	{
		auto animator = new FunctionalAnimator(action);
		node->addAnimator(animator);
		animator->drop();
	}

	FunctionalAnimator(Function action)
	{
		this->action = action;
	}

	virtual void animateNode(ISceneNode* node, u32 timeMs)
	{
		action(node, timeMs);
	}

	virtual ISceneNodeAnimator* createClone(ISceneNode* node,
	                                        ISceneManager* newManager)
	{
		FunctionalAnimator* newAnimator = new FunctionalAnimator(action);
		newAnimator->cloneMembers(this);
		return newAnimator;
	}

	Function action;
};

} // end namespace scene
} // end namespace irr
