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
 *   * This code incorporates portions of Open Dynamics Engine
 *     (Copyright (c) 2001-2004, Russell L. Smith. All rights
 *     reserved.) and portions of FCL (Copyright (c) 2011, Willow
 *     Garage, Inc. All rights reserved.), which were released under
 *     the same BSD license as below
 *
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

#ifndef DART_SIMULATION_DETAIL_WORLD_IMPL_HPP_
#define DART_SIMULATION_DETAIL_WORLD_IMPL_HPP_

#include <dart/simulation/World.hpp>

namespace dart {
namespace simulation {

//==============================================================================
template <typename... Args>
WorldPtr World::create(Args&&... args)
{
  return std::make_shared<World>(std::forward<Args>(args)...);
}

//==============================================================================
template <typename Func>
void World::eachSkeleton(Func func) const
{
  if constexpr (std::is_same_v<
                    std::invoke_result_t<Func, const dynamics::Skeleton*>,
                    bool>) {
    for (auto skel : mSkeletons) {
      if (!func(skel.get()))
        return;
    }
  } else {
    for (auto skel : mSkeletons) {
      func(skel.get());
    }
  }
}

//==============================================================================
template <typename Func>
void World::eachSkeleton(Func func)
{
  if constexpr (std::is_same_v<
                    std::invoke_result_t<Func, dynamics::Skeleton*>,
                    bool>) {
    for (auto skel : mSkeletons) {
      if (!func(skel.get()))
        return;
    }
  } else {
    for (auto skel : mSkeletons) {
      func(skel.get());
    }
  }
}

//==============================================================================
template <typename Func>
void World::eachSimpleFrame(Func func) const
{
  if constexpr (std::is_same_v<
                    std::invoke_result_t<Func, const dynamics::SimpleFrame*>,
                    bool>) {
    for (auto simpleFrame : mSimpleFrames) {
      if (!func(simpleFrame.get()))
        return;
    }
  } else {
    for (auto simpleFrame : mSimpleFrames) {
      func(simpleFrame.get());
    }
  }
}

//==============================================================================
template <typename Func>
void World::eachSimpleFrame(Func func)
{
  if constexpr (std::is_same_v<
                    std::invoke_result_t<Func, dynamics::SimpleFrame*>,
                    bool>) {
    for (auto simpleFrame : mSimpleFrames) {
      if (!func(simpleFrame.get()))
        return;
    }
  } else {
    for (auto simpleFrame : mSimpleFrames) {
      func(simpleFrame.get());
    }
  }
}

} // namespace simulation
} // namespace dart

#endif // DART_SIMULATION_DETAIL_WORLD_IMPL_HPP_
