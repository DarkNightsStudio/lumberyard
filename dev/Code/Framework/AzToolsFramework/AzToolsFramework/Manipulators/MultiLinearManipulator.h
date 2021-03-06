/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/

#pragma once

#include "BaseManipulator.h"
#include "LinearManipulator.h"

#include <AzToolsFramework/Manipulators/ManipulatorView.h>
#include <AzToolsFramework/Viewport/ViewportTypes.h>

namespace AzToolsFramework
{
    struct GridSnapAction;

    //! MultiLinearManipulator serves as a visual tool for users to modify values
    //! in one or more dimensions on axes defined in 3D space.
    class MultiLinearManipulator
        : public BaseManipulator
    {
        //! Private constructor.
        explicit MultiLinearManipulator(const AZ::Transform& worldFromLocal);

    public:
        AZ_CLASS_ALLOCATOR_DECL
        AZ_RTTI(MultiLinearManipulator, "{8490E883-8CC6-44C7-B2FE-AF9C9AF38AA7}", BaseManipulator)

        MultiLinearManipulator() = delete;
        MultiLinearManipulator(const MultiLinearManipulator&) = delete;
        MultiLinearManipulator& operator=(const MultiLinearManipulator&) = delete;

        ~MultiLinearManipulator();

        //! A Manipulator must only be created and managed through a shared_ptr.
        static AZStd::shared_ptr<MultiLinearManipulator> MakeShared(const AZ::Transform& worldFromLocal);

        //! Mouse action data used by MouseActionCallback
        //! Provides a collection of LinearManipulator actions for each axis.
        struct Action
        {
            AZStd::vector<LinearManipulator::Action> m_actions;
            int m_viewportId; //!< The id of the viewport this manipulator is being used in.
        };

        //! This is the function signature of callbacks that will be invoked whenever a MultiLinearManipulator
        //! is clicked on or dragged.
        using MouseActionCallback = AZStd::function<void(const Action&)>;

        void InstallLeftMouseDownCallback(const MouseActionCallback& onMouseDownCallback);
        void InstallMouseMoveCallback(const MouseActionCallback& onMouseMoveCallback);
        void InstallLeftMouseUpCallback(const MouseActionCallback& onMouseUpCallback);

        // BaseManipulator ...
        void Draw(
            const ManipulatorManagerState& managerState,
            AzFramework::DebugDisplayRequests& debugDisplay,
            const AzFramework::CameraState& cameraState,
            const ViewportInteraction::MouseInteraction& mouseInteraction) override;

        void AddAxis(const AZ::Vector3& axis);
        void AddAxes(const AZStd::vector<AZ::Vector3>& axes);
        void ClearAxes();

        void SetSpace(const AZ::Transform& worldFromLocal);
        void SetLocalTransform(const AZ::Transform& localTransform);
        void SetLocalPosition(const AZ::Vector3& localPosition);
        void SetLocalOrientation(const AZ::Quaternion& localOrientation);

        AZ::Vector3 GetLocalPosition() const;
        const AZ::Transform& GetSpace() const;
        const AZ::Transform& GetLocalTransform() const;

        using ConstFixedIterator = AZStd::vector<LinearManipulator::Fixed>::const_iterator;
        ConstFixedIterator FixedBegin() const;
        ConstFixedIterator FixedEnd() const;

        template<typename Views>
        void SetViews(Views&& views)
        {
            m_manipulatorViews = AZStd::forward<Views>(views);
        }

    private:
        void OnLeftMouseDownImpl(
            const ViewportInteraction::MouseInteraction& interaction, float rayIntersectionDistance) override;
        void OnLeftMouseUpImpl(
            const ViewportInteraction::MouseInteraction& interaction) override;
        void OnMouseMoveImpl(
            const ViewportInteraction::MouseInteraction& interaction) override;

        void InvalidateImpl() override;
        void SetBoundsDirtyImpl() override;

        AZ::Transform m_localTransform = AZ::Transform::CreateIdentity(); //!< Local transform of the manipulator.
        AZ::Transform m_worldFromLocal = AZ::Transform::CreateIdentity(); //!< Space the manipulator is in (identity is world space).

        AZStd::vector<LinearManipulator::Fixed> m_fixedAxes; //!< A collection of LinearManipulator fixed states.
        AZStd::vector<LinearManipulator::Starter> m_starters; //!< A collection of LinearManipulator starter states.

        MouseActionCallback m_onLeftMouseDownCallback = nullptr;
        MouseActionCallback m_onLeftMouseUpCallback = nullptr;
        MouseActionCallback m_onMouseMoveCallback = nullptr;

        ManipulatorViews m_manipulatorViews; //!< Look of manipulator.
    };

    inline AZ::Vector3 MultiLinearManipulator::GetLocalPosition() const
    {
        return m_localTransform.GetTranslation();
    }

    inline const AZ::Transform& MultiLinearManipulator::GetSpace() const
    {
        return m_localTransform;
    }

    inline const AZ::Transform& MultiLinearManipulator::GetLocalTransform() const
    {
        return m_localTransform;
    }

    inline MultiLinearManipulator::ConstFixedIterator MultiLinearManipulator::FixedBegin() const
    {
        return m_fixedAxes.cbegin();
    }

    inline MultiLinearManipulator::ConstFixedIterator MultiLinearManipulator::FixedEnd() const
    {
        return m_fixedAxes.cend();
    }
} // namespace AzToolsFramework
