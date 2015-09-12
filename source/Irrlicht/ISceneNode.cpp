// Copyright (C) 2002-2012 Nikolaus Gebhardt
// This file is part of the "Irrlicht Engine".
// For conditions of distribution and use, see copyright notice in irrlicht.h

#include "ISceneNode.h"

namespace irr
{
    namespace scene
    {

        ISceneNode::ISceneNode(ISceneNode* parent, ISceneManager* mgr, s32 id,
            const core::vector3df& position,
            const core::vector3df& rotation,
            const core::vector3df& scale)
            : RelativeTranslation(position), RelativeRotation(rotation), RelativeScale(scale),
            Parent(0), SceneManager(mgr), TriangleSelector(0), ID(id),
            AutomaticCullingState(EAC_BOX), DebugDataVisible(EDS_OFF),
            IsVisible(true), IsDebugObject(false), IsBillboard(false)
        {
            if (parent)
                parent->addChild(this);

            updateAbsolutePosition();
        }


        //! Destructor
        ISceneNode::~ISceneNode()
        {
            // delete all children
            removeAll();

            // delete all animators
            ISceneNodeAnimatorList::Iterator ait = Animators.begin();
            for (; ait != Animators.end(); ++ait)
                (*ait)->drop();

            if (TriangleSelector)
                TriangleSelector->drop();
        }


        //! This method is called just before the rendering process of the whole scene.
        /** Nodes may register themselves in the render pipeline during this call,
        precalculate the geometry which should be renderered, and prevent their
        children from being able to register themselves if they are clipped by simply
        not calling their OnRegisterSceneNode method.
        If you are implementing your own scene node, you should overwrite this method
        with an implementation code looking like this:
        \code
        if (IsVisible)
        SceneManager->registerNodeForRendering(this);

        ISceneNode::OnRegisterSceneNode();
        \endcode
        */
        void ISceneNode::OnRegisterSceneNode()
        {
            if (IsVisible)
            {
                //printf("%s\n", getDebugName());
                ISceneNodeList::Iterator it = Children.begin();
                for (; it != Children.end(); ++it)
                    (*it)->OnRegisterSceneNode();
            }
        }


        //! OnAnimate() is called just before rendering the whole scene.
        /** Nodes may calculate or store animations here, and may do other useful things,
        depending on what they are. Also, OnAnimate() should be called for all
        child scene nodes here. This method will be called once per frame, independent
        of whether the scene node is visible or not.
        \param timeMs Current time in milliseconds. */
        void ISceneNode::OnAnimate(u32 timeMs)
        {
            if (IsVisible)
            {
                // animate this node with all animators

                ISceneNodeAnimatorList::Iterator ait = Animators.begin();
                while (ait != Animators.end())
                {
                    // continue to the next node before calling animateNode()
                    // so that the animator may remove itself from the scene
                    // node without the iterator becoming invalid
                    ISceneNodeAnimator* anim = *ait;
                    ++ait;
                    if (anim->isEnabled())
                    {
                        anim->animateNode(this, timeMs);
                    }
                }

                // update absolute position
                updateAbsolutePosition();

                // perform the post render process on all children

                ISceneNodeList::Iterator it = Children.begin();
                for (; it != Children.end(); ++it)
                    (*it)->OnAnimate(timeMs);
            }
        }



        //! Returns the name of the node.
        /** \return Name as character string. */
        const c8* ISceneNode::getName() const
        {
            return Name.c_str();
        }


        //! Sets the name of the node.
        /** \param name New name of the scene node. */
        void ISceneNode::setName(const c8* name)
        {
            Name = name;
        }


        //! Sets the name of the node.
        /** \param name New name of the scene node. */
        void ISceneNode::setName(const core::stringc& name)
        {
            Name = name;
        }



        //! Get the axis aligned, transformed and animated absolute bounding box of this node.
        /** Note: The result is still an axis-aligned bounding box, so it's size
        changes with rotation.
        \return The transformed bounding box. */
        const core::aabbox3d<f32> ISceneNode::getTransformedBoundingBox() const
        {
            core::aabbox3d<f32> box = getBoundingBox();
            AbsoluteTransformation.transformBoxEx(box);
            return box;
        }

        //! Get a the 8 corners of the original bounding box transformed and
        //! animated by the absolute transformation.
        /** Note: The result is _not_ identical to getTransformedBoundingBox().getEdges(),
        but getting an aabbox3d of these edges would then be identical.
        \param edges Receives an array with the transformed edges */
        void ISceneNode::getTransformedBoundingBoxEdges(core::array< core::vector3d<f32> >& edges) const
        {
            edges.set_used(8);
            getBoundingBox().getEdges(edges.pointer());
            for (u32 i = 0; i < 8; ++i)
                AbsoluteTransformation.transformVect(edges[i]);
        }

        //! Get the absolute transformation of the node. Is recalculated every OnAnimate()-call.
        /** NOTE: For speed reasons the absolute transformation is not
        automatically recalculated on each change of the relative
        transformation or by a transformation change of an parent. Instead the
        update usually happens once per frame in OnAnimate. You can enforce
        an update with updateAbsolutePosition().
        \return The absolute transformation matrix. */
        const core::matrix4& ISceneNode::getAbsoluteTransformation() const
        {
            return AbsoluteTransformation;
        }


        //! Returns the relative transformation of the scene node.
        /** The relative transformation is stored internally as 3
        vectors: translation, rotation and scale. To get the relative
        transformation matrix, it is calculated from these values.
        \return The relative transformation matrix. */
        core::matrix4 ISceneNode::getRelativeTransformation() const
        {
            core::matrix4 mat;
            mat.setRotationDegrees(RelativeRotation);
            mat.setTranslation(RelativeTranslation);

            if (RelativeScale != core::vector3df(1.f, 1.f, 1.f))
            {
                core::matrix4 smat;
                smat.setScale(RelativeScale);
                mat *= smat;
            }

            return mat;
        }


        //! Returns whether the node should be visible (if all of its parents are visible).
        /** This is only an option set by the user, but has nothing to
        do with geometry culling
        \return The requested visibility of the node, true means
        visible (if all parents are also visible). */
        bool ISceneNode::isVisible() const
        {
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
            return IsVisible;
        }

        //! Check whether the node is truly visible, taking into accounts its parents' visibility
        /** \return true if the node and all its parents are visible,
        false if this or any parent node is invisible. */
        bool ISceneNode::isTrulyVisible() const
        {
            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
            if (!IsVisible)
                return false;

            if (!Parent)
                return true;

            return Parent->isTrulyVisible();
        }

        //! Sets if the node should be visible or not.
        /** All children of this node won't be visible either, when set
        to false. Invisible nodes are not valid candidates for selection by
        collision manager bounding box methods.
        \param isVisible If the node shall be visible. */
        void ISceneNode::setVisible(bool isVisible)
        {
            IsVisible = isVisible;
        }


        //! Get the id of the scene node.
        /** This id can be used to identify the node.
        \return The id. */
        s32 ISceneNode::getID() const
        {
            return ID;
        }


        //! Sets the id of the scene node.
        /** This id can be used to identify the node.
        \param id The new id. */
        void ISceneNode::setID(s32 id)
        {
            ID = id;
        }


        //! Adds a child to this scene node.
        /** If the scene node already has a parent it is first removed
        from the other parent.
        \param child A pointer to the new child. */
        void ISceneNode::addChild(ISceneNode* child)
        {
            if (child && (child != this))
            {
                // Change scene manager?
                if (SceneManager != child->SceneManager)
                    child->setSceneManager(SceneManager);

                child->grab();
                child->remove(); // remove from old parent
                Children.push_back(child);
                child->Parent = this;
            }
        }


        //! Removes a child from this scene node.
        /** If found in the children list, the child pointer is also
        dropped and might be deleted if no other grab exists.
        \param child A pointer to the child which shall be removed.
        \return True if the child was removed, and false if not,
        e.g. because it couldn't be found in the children list. */
        bool ISceneNode::removeChild(ISceneNode* child)
        {
            ISceneNodeList::Iterator it = Children.begin();
            for (; it != Children.end(); ++it)
                if ((*it) == child)
                {
                (*it)->Parent = 0;
                (*it)->drop();
                Children.erase(it);
                return true;
                }

            _IRR_IMPLEMENT_MANAGED_MARSHALLING_BUGFIX;
            return false;
        }


        //! Removes all children of this scene node
        /** The scene nodes found in the children list are also dropped
        and might be deleted if no other grab exists on them.
        */
        void ISceneNode::removeAll()
        {
            ISceneNodeList::Iterator it = Children.begin();
            for (; it != Children.end(); ++it)
            {
                (*it)->Parent = 0;
                (*it)->drop();
            }

            Children.clear();
        }


        //! Removes this scene node from the scene
        /** If no other grab exists for this node, it will be deleted.
        */
        void ISceneNode::remove()
        {
            if (Parent)
                Parent->removeChild(this);
        }


        //! Adds an animator which should animate this node.
        /** \param animator A pointer to the new animator. */
        void ISceneNode::addAnimator(ISceneNodeAnimator* animator)
        {
            if (animator)
            {
                Animators.push_back(animator);
                animator->grab();
            }
        }


        //! Removes an animator from this scene node.
        /** If the animator is found, it is also dropped and might be
        deleted if not other grab exists for it.
        \param animator A pointer to the animator to be deleted. */
        void ISceneNode::removeAnimator(ISceneNodeAnimator* animator)
        {
            ISceneNodeAnimatorList::Iterator it = Animators.begin();
            for (; it != Animators.end(); ++it)
            {
                if ((*it) == animator)
                {
                    (*it)->drop();
                    Animators.erase(it);
                    return;
                }
            }
        }


        //! Removes all animators from this scene node.
        /** The animators might also be deleted if no other grab exists
        for them. */
        void ISceneNode::removeAnimators()
        {
            ISceneNodeAnimatorList::Iterator it = Animators.begin();
            for (; it != Animators.end(); ++it)
                (*it)->drop();

            Animators.clear();
        }


        //! Returns the material based on the zero based index i.
        /** To get the amount of materials used by this scene node, use
        getMaterialCount(). This function is needed for inserting the
        node into the scene hierarchy at an optimal position for
        minimizing renderstate changes, but can also be used to
        directly modify the material of a scene node.
        \param num Zero based index. The maximal value is getMaterialCount() - 1.
        \return The material at that index. */
        video::SMaterial& ISceneNode::getMaterial(u32 num)
        {
            return video::IdentityMaterial;
        }


        //! Get amount of materials used by this scene node.
        /** \return Current amount of materials of this scene node. */
        u32 ISceneNode::getMaterialCount() const
        {
            return 0;
        }

        //! Gets the scale of the scene node relative to its parent.
        /** This is the scale of this node relative to its parent.
        If you want the absolute scale, use
        getAbsoluteTransformation().getScale()
        \return The scale of the scene node. */
        const core::vector3df& ISceneNode::getScale() const
        {
            return RelativeScale;
        }


        //! Sets the relative scale of the scene node.
        /** \param scale New scale of the node, relative to its parent. */
        void ISceneNode::setScale(const core::vector3df& scale)
        {
            RelativeScale = scale;
        }


        //! Gets the rotation of the node relative to its parent.
        /** Note that this is the relative rotation of the node.
        If you want the absolute rotation, use
        getAbsoluteTransformation().getRotation()
        \return Current relative rotation of the scene node. */
        const core::vector3df& ISceneNode::getRotation() const
        {
            return RelativeRotation;
        }


        //! Sets the rotation of the node relative to its parent.
        /** This only modifies the relative rotation of the node.
        \param rotation New rotation of the node in degrees. */
        void ISceneNode::setRotation(const core::vector3df& rotation)
        {
            RelativeRotation = rotation;
        }


        //! Gets the position of the node relative to its parent.
        /** Note that the position is relative to the parent. If you want
        the position in world coordinates, use getAbsolutePosition() instead.
        \return The current position of the node relative to the parent. */
        const core::vector3df& ISceneNode::getPosition() const
        {
            return RelativeTranslation;
        }


        //! Sets the position of the node relative to its parent.
        /** Note that the position is relative to the parent.
        \param newpos New relative position of the scene node. */
        void ISceneNode::setPosition(const core::vector3df& newpos)
        {
            RelativeTranslation = newpos;
        }


        //! Gets the absolute position of the node in world coordinates.
        /** If you want the position of the node relative to its parent,
        use getPosition() instead.
        NOTE: For speed reasons the absolute position is not
        automatically recalculated on each change of the relative
        position or by a position change of an parent. Instead the
        update usually happens once per frame in OnAnimate. You can enforce
        an update with updateAbsolutePosition().
        \return The current absolute position of the scene node (updated on last call of updateAbsolutePosition). */
        core::vector3df ISceneNode::getAbsolutePosition() const
        {
            return AbsoluteTransformation.getTranslation();
        }




        //! Sets if debug data like bounding boxes should be drawn.
        /** A bitwise OR of the types from @ref irr::scene::E_DEBUG_SCENE_TYPE.
        Please note that not all scene nodes support all debug data types.
        \param state The debug data visibility state to be used. */
        void ISceneNode::setDebugDataVisible(u32 state)
        {
            DebugDataVisible = state;
        }


        //! Changes the parent of the scene node.
        /** \param newParent The new parent to be used. */
        void ISceneNode::setParent(ISceneNode* newParent)
        {
            grab();
            remove();

            Parent = newParent;

            if (Parent)
                Parent->addChild(this);

            drop();
        }


        //! Returns the triangle selector attached to this scene node.
        /** The Selector can be used by the engine for doing collision
        detection. You can create a TriangleSelector with
        ISceneManager::createTriangleSelector() or
        ISceneManager::createOctreeTriangleSelector and set it with
        ISceneNode::setTriangleSelector(). If a scene node got no triangle
        selector, but collision tests should be done with it, a triangle
        selector is created using the bounding box of the scene node.
        \return A pointer to the TriangleSelector or 0, if there
        is none. */
        ITriangleSelector* ISceneNode::getTriangleSelector() const
        {
            return TriangleSelector;
        }


        //! Sets the triangle selector of the scene node.
        /** The Selector can be used by the engine for doing collision
        detection. You can create a TriangleSelector with
        ISceneManager::createTriangleSelector() or
        ISceneManager::createOctreeTriangleSelector(). Some nodes may
        create their own selector by default, so it would be good to
        check if there is already a selector in this node by calling
        ISceneNode::getTriangleSelector().
        \param selector New triangle selector for this scene node. */
        void ISceneNode::setTriangleSelector(ITriangleSelector* selector)
        {
            if (TriangleSelector != selector)
            {
                if (TriangleSelector)
                    TriangleSelector->drop();

                TriangleSelector = selector;
                if (TriangleSelector)
                    TriangleSelector->grab();
            }
        }


        //! Updates the absolute position based on the relative and the parents position
        /** Note: This does not recursively update the parents absolute positions, so if you have a deeper
            hierarchy you might want to update the parents first.*/
        void ISceneNode::updateAbsolutePosition()
        {
            if (Parent)
            {
                AbsoluteTransformation =
                    Parent->getAbsoluteTransformation() * getRelativeTransformation();
            }
            else
                AbsoluteTransformation = getRelativeTransformation();
        }



        //! Returns type of the scene node
        /** \return The type of this node. */
        ESCENE_NODE_TYPE ISceneNode::getType() const
        {
            return ESNT_UNKNOWN;
        }


        //! Writes attributes of the scene node.
        /** Implement this to expose the attributes of your scene node
        for scripting languages, editors, debuggers or xml
        serialization purposes.
        \param out The attribute container to write into.
        \param options Additional options which might influence the
        serialization. */
        void ISceneNode::serializeAttributes(io::IAttributes* out, io::SAttributeReadWriteOptions* options) const
        {
            if (!out)
                return;
            out->addString("Name", Name.c_str());
            out->addInt("Id", ID);

            out->addVector3d("Position", getPosition());
            out->addVector3d("Rotation", getRotation());
            out->addVector3d("Scale", getScale());

            out->addBool("Visible", IsVisible);
            out->addInt("AutomaticCulling", AutomaticCullingState);
            out->addInt("DebugDataVisible", DebugDataVisible);
            out->addBool("IsDebugObject", IsDebugObject);
        }


        //! Reads attributes of the scene node.
        /** Implement this to set the attributes of your scene node for
        scripting languages, editors, debuggers or xml deserialization
        purposes.
        \param in The attribute container to read from.
        \param options Additional options which might influence the
        deserialization. */
        void ISceneNode::deserializeAttributes(io::IAttributes* in, io::SAttributeReadWriteOptions* options)
        {
            if (!in)
                return;
            Name = in->getAttributeAsString("Name", Name);
            ID = in->getAttributeAsInt("Id", ID);

            setPosition(in->getAttributeAsVector3d("Position", RelativeTranslation));
            setRotation(in->getAttributeAsVector3d("Rotation", RelativeRotation));
            setScale(in->getAttributeAsVector3d("Scale", RelativeRotation));

            IsVisible = in->getAttributeAsBool("Visible", IsVisible);
            if (in->existsAttribute("AutomaticCulling"))
            {
                s32 tmpState = in->getAttributeAsEnumeration("AutomaticCulling",
                    scene::AutomaticCullingNames);
                if (tmpState != -1)
                    AutomaticCullingState = (u32)tmpState;
                else
                    AutomaticCullingState = in->getAttributeAsInt("AutomaticCulling");
            }

            DebugDataVisible = in->getAttributeAsInt("DebugDataVisible", DebugDataVisible);
            IsDebugObject = in->getAttributeAsBool("IsDebugObject", IsDebugObject);

            updateAbsolutePosition();
        }

        //! Creates a clone of this scene node and its children.
        /** \param newParent An optional new parent.
        \param newManager An optional new scene manager.
        \return The newly created clone of this node. */
        ISceneNode* ISceneNode::clone(ISceneNode* newParent, ISceneManager* newManager )
        {
            return 0; // to be implemented by derived classes
        }

        //! Retrieve the scene manager for this node.
        /** \return The node's scene manager. */
        ISceneManager* ISceneNode::getSceneManager(void) const { return SceneManager; }


        //! A clone function for the ISceneNode members.
        /** This method can be used by clone() implementations of
        derived classes
        \param toCopyFrom The node from which the values are copied
        \param newManager The new scene manager. */
        void ISceneNode::cloneMembers(ISceneNode* toCopyFrom, ISceneManager* newManager)
        {
            Name = toCopyFrom->Name;
            AbsoluteTransformation = toCopyFrom->AbsoluteTransformation;
            RelativeTranslation = toCopyFrom->RelativeTranslation;
            RelativeRotation = toCopyFrom->RelativeRotation;
            RelativeScale = toCopyFrom->RelativeScale;
            ID = toCopyFrom->ID;
            setTriangleSelector(toCopyFrom->TriangleSelector);
            AutomaticCullingState = toCopyFrom->AutomaticCullingState;
            DebugDataVisible = toCopyFrom->DebugDataVisible;
            IsVisible = toCopyFrom->IsVisible;
            IsDebugObject = toCopyFrom->IsDebugObject;

            if (newManager)
                SceneManager = newManager;
            else
                SceneManager = toCopyFrom->SceneManager;

            // clone children

            ISceneNodeList::Iterator it = toCopyFrom->Children.begin();
            for (; it != toCopyFrom->Children.end(); ++it)
                (*it)->clone(this, newManager);

            // clone animators

            ISceneNodeAnimatorList::Iterator ait = toCopyFrom->Animators.begin();
            for (; ait != toCopyFrom->Animators.end(); ++ait)
            {
                ISceneNodeAnimator* anim = (*ait)->createClone(this, SceneManager);
                if (anim)
                {
                    addAnimator(anim);
                    anim->drop();
                }
            }
        }

        //! Sets the new scene manager for this node and all children.
        //! Called by addChild when moving nodes between scene managers
        void ISceneNode::setSceneManager(ISceneManager* newManager)
        {
            SceneManager = newManager;

            ISceneNodeList::Iterator it = Children.begin();
            for (; it != Children.end(); ++it)
                (*it)->setSceneManager(newManager);
        }
    }
}
