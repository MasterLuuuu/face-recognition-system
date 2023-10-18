/**
 * Copyright(c) Live2D Inc. All rights reserved.
 *
 * Use of this source code is governed by the Live2D Open Software license
 * that can be found at https://www.live2d.com/eula/live2d-open-software-license-agreement_en.html.
 */

#include <windows.h>

#include "LAppDelegate.hpp"
#include "LAppInterface.hpp"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    LAppInterface::GetInstance()->InitializeDelegate(); 
    LAppInterface::GetInstance()->SetMotion(MotionType::BIGSMILE);
    return 0;
}
