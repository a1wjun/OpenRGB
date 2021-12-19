#include "Detector.h"
#include "DygmaRaiseController.h"
#include "RGBController.h"
#include "RGBController_DygmaRaise.h"
#include "find_usb_serial_port.h"
#include <vector>
#include <stdio.h>


#define DYGMA_RAISE_VID 0x1209
#define DYGMA_RAISE_PID 0x2201

/******************************************************************************************\
*                                                                                          *
*   DetectDygmaRaiseControllers                                                            *
*                                                                                          *
*       Tests the USB address to see if a DygmaRaise keyboard exists there.                *
*       Then opens a serial port to communicate with the KB                                *
*                                                                                          *
\******************************************************************************************/

void DetectDygmaRaiseControllers(std::vector<RGBController*> &rgb_controllers)
{
    std::vector<std::string *> ports = find_usb_serial_port(DYGMA_RAISE_VID, DYGMA_RAISE_PID);

    for(std::size_t i = 0; i < ports.size(); i++)
    {
        if(*ports[i] != "")
        {
            DygmaRaiseController*     controller     = new DygmaRaiseController();
            controller->Initialize((char *)ports[i]->c_str());

            RGBController_DygmaRaise* rgb_controller = new RGBController_DygmaRaise(controller);
            rgb_controllers.push_back(rgb_controller);
        }
    }
}

REGISTER_DETECTOR("Dygma Raise", DetectDygmaRaiseControllers);
