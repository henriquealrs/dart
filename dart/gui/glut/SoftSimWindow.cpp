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

#include "dart/gui/glut/SoftSimWindow.hpp"

#include "dart/gui/glut/LoadGlut.hpp"

namespace dart {
namespace gui {
namespace glut {

SoftSimWindow::SoftSimWindow()
  : SimWindow(), mShowPointMasses(false), mShowMeshs(true)
{
}

SoftSimWindow::~SoftSimWindow() {}

void SoftSimWindow::keyboard(unsigned char key, int x, int y)
{
  switch (key) {
    case ' ': // use space key to play or stop the motion
      mSimulating = !mSimulating;
      if (mSimulating)
        mPlay = false;
      break;
    case 'p': // playBack
      mPlay = !mPlay;
      if (mPlay)
        mSimulating = false;
      break;
    // case '[':  // step backward
    //   if (!mSimulating)
    //   {
    //     mPlayFrame--;
    //     if (mPlayFrame < 0)
    //       mPlayFrame = 0;
    //     glutPostRedisplay();
    //   }
    //   break;
    // case ']':  // step forwardward
    //   if (!mSimulating)
    //   {
    //     mPlayFrame++;
    //     if (mPlayFrame >= mWorld->getRecording()->getNumFrames())
    //       mPlayFrame = 0;
    //     glutPostRedisplay();
    //   }
    //   break;
    case 'v': // show or hide markers
      mShowMarkers = !mShowMarkers;
      break;
    case 'n':
      mShowPointMasses = !mShowPointMasses;
      break;
    case 'm':
      mShowMeshs = !mShowMeshs;
      break;
    default:
      Win3D::keyboard(key, x, y);
  }
  glutPostRedisplay();
}

} // namespace glut
} // namespace gui
} // namespace dart
