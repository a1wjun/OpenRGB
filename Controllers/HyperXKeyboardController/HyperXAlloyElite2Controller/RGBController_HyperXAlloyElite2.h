/*---------------------------------------------------------*\
| RGBController_HyperXAlloyElite2.h                         |
|                                                           |
|   RGBController for HyperX Alloy Elite 2 keyboard         |
|                                                           |
|   KundaPanda (vojdo)                          02 Apr 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#pragma once

#include <atomic>
#include <chrono>
#include <thread>
#include "RGBController.h"
#include "HyperXAlloyElite2Controller.h"

class RGBController_HyperXAlloyElite2 : public RGBController
{
public:
    RGBController_HyperXAlloyElite2(HyperXAlloyElite2Controller* controller_ptr);
    ~RGBController_HyperXAlloyElite2();

    void        SetupZones();

    void        ResizeZone(int zone, int new_size);

    void        DeviceUpdateLEDs();
    void        UpdateZoneLEDs(int zone);
    void        UpdateSingleLED(int led);

    void        DeviceUpdateMode();

    void        KeepaliveThreadFunction();

private:
    HyperXAlloyElite2Controller*                        controller;
    std::atomic<bool>                                   keepalive_thread_run;
    std::thread*                                        keepalive_thread;
    std::chrono::time_point<std::chrono::steady_clock>  last_update_time;
};
