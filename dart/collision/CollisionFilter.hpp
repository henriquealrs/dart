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

#ifndef DART_COLLISION_COLLISIONFILTER_HPP_
#define DART_COLLISION_COLLISIONFILTER_HPP_

#include <dart/collision/detail/UnorderedPairs.hpp>

#include <dart/common/Deprecated.hpp>

namespace dart {

namespace dynamics {
class BodyNode;
} // namespace dynamics

namespace collision {

class CollisionObject;

class CollisionFilter
{
public:
  /// Destructor.
  virtual ~CollisionFilter();

  /// Returns true if the given two CollisionObjects should be checked by the
  /// collision detector, false otherwise.
  /// \deprecated Deprecated in 6.3.0. Please use ignoreCollision instead. Note
  /// that ignoreCollision returns logically opposite to what needCollision
  /// returns.
  DART_DEPRECATED(6.3)
  bool needCollision(
      const CollisionObject* object1, const CollisionObject* object2) const;

  /// Returns true if the given two CollisionObjects should be checked by the
  /// collision detector, false otherwise.
  virtual bool ignoresCollision(
      const CollisionObject* object1, const CollisionObject* object2) const = 0;
};

class CompositeCollisionFilter : public CollisionFilter
{
public:
  /// Adds a collision filter to this CompositeCollisionFilter.
  void addCollisionFilter(const CollisionFilter* filter);

  /// Removes a collision filter from this CompositeCollisionFilter.
  void removeCollisionFilter(const CollisionFilter* filter);

  /// Removes all the collision filters from this CompositeCollisionFilter.
  void removeAllCollisionFilters();

  // Documentation inherited
  bool ignoresCollision(
      const CollisionObject* object1,
      const CollisionObject* object2) const override;

protected:
  /// Collision filters
  std::unordered_set<const CollisionFilter*> mFilters;
};

class BodyNodeCollisionFilter : public CollisionFilter
{
public:
  /// Add a BodyNode pair to the blacklist.
  void addBodyNodePairToBlackList(
      const dynamics::BodyNode* bodyNode1, const dynamics::BodyNode* bodyNode2);

  /// Remove a BodyNode pair from the blacklist.
  void removeBodyNodePairFromBlackList(
      const dynamics::BodyNode* bodyNode1, const dynamics::BodyNode* bodyNode2);

  /// Remove all the BodyNode pairs from the blacklist.
  void removeAllBodyNodePairsFromBlackList();

  // Documentation inherited
  bool ignoresCollision(
      const CollisionObject* object1,
      const CollisionObject* object2) const override;

private:
  /// Returns true if the two BodyNodes are adjacent BodyNodes (i.e., the two
  /// BodyNodes are connected by a Joint).
  bool areAdjacentBodies(
      const dynamics::BodyNode* bodyNode1,
      const dynamics::BodyNode* bodyNode2) const;

  /// List of pairs to be ignored in the collision detection.
  detail::UnorderedPairs<dynamics::BodyNode> mBodyNodeBlackList;
};

} // namespace collision
} // namespace dart

#endif // DART_COLLISION_COLLISIONFILTER_HPP_
