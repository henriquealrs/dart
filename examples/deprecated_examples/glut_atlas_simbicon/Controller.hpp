/*
 * Copyright (c) 2011-2025, The DART development contributors
 * All rights reserved.
 *
 * The list of contributors can be found at:
 *   https://github.com/dartsim/dart/blob/main/LICENSE
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef EXAMPLES_ATLASSIMBICON_CONTROLLER_HPP_
#define EXAMPLES_ATLASSIMBICON_CONTROLLER_HPP_

#include <dart/dart.hpp>

#include <Eigen/Dense>

#include <vector>

class StateMachine;

/// \brief SIMBICON for Atlas robot
class Controller
{
public:
  /// \brief Constructor
  Controller(
      dart::dynamics::SkeletonPtr _atlasRobot,
      dart::constraint::ConstraintSolver* _collisionSolver);

  /// \brief Destructor
  virtual ~Controller();

  /// \brief Called before every simulation time step in MyWindow class.
  /// Compute control force and apply it to Atlas robot
  virtual void update(double _currentTime);

  /// \brief
  dart::dynamics::SkeletonPtr getAtlasRobot();

  /// \brief Get current state machine
  StateMachine* getCurrentState();

  /// \brief Change state to _stateMachine
  void changeStateMachine(StateMachine* _stateMachine, double _currentTime);

  /// \brief Change state machine to a state machine whose names is _name
  void changeStateMachine(const std::string& _name, double _currentTime);

  /// \brief Change state machine to a state machine whose index is _idx
  void changeStateMachine(std::size_t _idx, double _currentTime);

  /// \brief Keyboard control
  void keyboard(unsigned char _key, int _x, int _y, double _currentTime);

  /// \brief Print debug information
  void printDebugInfo() const;

  /// \brief Harness the robot
  void harnessPelvis();

  /// \brief Unharness the robot
  void unharnessPelvis();

  /// \brief Harness the robot
  void harnessLeftFoot();

  /// \brief Harness the robot
  void unharnessLeftFoot();

  /// \brief Harness the robot
  void harnessRightFoot();

  /// \brief Harness the robot
  void unharnessRightFoot();

  /// \brief Reset the robot
  void resetRobot();

protected:
  /// \brief Atlas robot skeleton
  dart::dynamics::SkeletonPtr mAtlasRobot;

  /// \brief Conllision detector
  dart::constraint::ConstraintSolver* mConstratinSolver;

  /// \brief List of state machines
  std::vector<StateMachine*> mStateMachines;

  /// \brief Current state machine
  StateMachine* mCurrentStateMachine;

  /// \brief Flag for pelvis harnessing
  bool mPelvisHarnessOn;

  /// \brief Flag for left foot harnessing
  bool mLeftFootHarnessOn;

  /// \brief Flag for right foot harnessing
  bool mRightFootHarnessOn;

  /// \brief Index for coronal left hip
  std::size_t mCoronalLeftHip;

  /// \brief Index for coronal right hip
  std::size_t mCoronalRightHip;

  /// \brief Index for sagital left hip
  std::size_t mSagitalLeftHip;

  /// \brief Index for sagital right hip
  std::size_t mSagitalRightHip;

private:
  /// \brief Check if this controller contains _stateMachine
  bool _containStateMachine(const StateMachine* _stateMachine) const;

  /// \brief Check if this controller contains a state machine whose name is
  ///        _name
  bool _containStateMachine(const std::string& _name) const;

  /// \brief Find a state machine whose name is _name
  StateMachine* _findStateMachine(const std::string& _name) const;

  /// \brief Build state machines
  void _buildStateMachines();

  /// \brief Create standing controller
  StateMachine* _createStandingStateMachine();

  /// \brief Create standing controller
  StateMachine* _createWalkingInPlaceStateMachine();

  /// \brief Create standing controller
  StateMachine* _createWalkingStateMachine();

  /// \brief Create running controller
  StateMachine* _createRunningStateMachine();

  /// \brief Set joint damping
  void _setJointDamping();

  /// \brief Get left foot
  dart::dynamics::BodyNode* _getLeftFoot() const;

  /// \brief Get right foot
  dart::dynamics::BodyNode* _getRightFoot() const;

  /// \brief Weld joint constraint for pelvis harnessing
  dart::constraint::WeldJointConstraintPtr mWeldJointConstraintPelvis;

  /// \brief Weld joint constraint for left foot harnessing
  dart::constraint::WeldJointConstraintPtr mWeldJointConstraintLeftFoot;

  /// \brief Weld joint constraint for right foot harnessing
  dart::constraint::WeldJointConstraintPtr mWeldJointConstraintRightFoot;

  /// \brief Initial state of the robot
  dart::dynamics::Skeleton::Configuration mInitialState;
};

#endif // EXAMPLES_ATLASSIMBICON_CONTROLLER_HPP_
