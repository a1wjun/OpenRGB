/*---------------------------------------------------------*\
| LogitechGProController.cpp                                |
|                                                           |
|   Driver for Logitech G Pro keyboard                      |
|                                                           |
|   sanchezzzs                                  20 Oct 2021 |
|                                                           |
|   This file is part of the OpenRGB project                |
|   SPDX-License-Identifier: GPL-2.0-only                   |
\*---------------------------------------------------------*/

#include <cstring>
#include "LogitechGProKeyboardController.h"
#include "StringUtils.h"

LogitechGProKeyboardController::LogitechGProKeyboardController(hid_device* dev_handle_0x11, hid_device* dev_handle_0x12)
{
    dev_pkt_0x11 = dev_handle_0x11;
    dev_pkt_0x12 = dev_handle_0x12;
}

LogitechGProKeyboardController::~LogitechGProKeyboardController()
{
    hid_close(dev_pkt_0x11);
    hid_close(dev_pkt_0x12);
}

std::string LogitechGProKeyboardController::GetSerialString()
{
    wchar_t serial_string[128];
    int ret = hid_get_serial_number_string(dev_pkt_0x11, serial_string, 128);

    if(ret != 0)
    {
        return("");
    }

    return(StringUtils::wstring_to_string(serial_string));
}

void LogitechGProKeyboardController::Commit()
{
    SendCommit();
}

void LogitechGProKeyboardController::SetDirect
    (
    unsigned char       zone,
    unsigned char       frame_count,
    unsigned char *     frame_data
    )
{
    SendDirectFrame(zone, frame_count, frame_data);
}

void LogitechGProKeyboardController::SetMode
    (
    unsigned char       mode,
    unsigned short      speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    SendMode(LOGITECH_GPRO_ZONE_MODE_KEYBOARD, mode, speed, red, green, blue);
    SendMode(LOGITECH_GPRO_ZONE_MODE_LOGO,     mode, speed, red, green, blue);

    SendCommit();
}

/*-------------------------------------------------------------------------------------------------*\
| Private packet sending functions.                                                                 |
\*-------------------------------------------------------------------------------------------------*/

void LogitechGProKeyboardController::SendCommit()
{
    unsigned char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Commit packet                                  |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0C;
    usb_buf[0x03]           = 0x5D;

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, usb_buf, 20);
    hid_read(dev_pkt_0x11, usb_buf, 20);
}

void LogitechGProKeyboardController::SendDirectFrame
    (
    unsigned char       zone,
    unsigned char       frame_count,
    unsigned char *     frame_data
    )
{
    unsigned char usb_buf[64];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x12;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0C;
    usb_buf[0x03]           = 0x3D;
    usb_buf[0x05]           = zone;
    usb_buf[0x07]           = frame_count;

    /*-----------------------------------------------------*\
    | Copy in frame data                                    |
    \*-----------------------------------------------------*/
    memcpy(&usb_buf[0x08], frame_data, frame_count * 4);

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x12, usb_buf, 64);
    hid_read(dev_pkt_0x11, usb_buf, 20);
}

void LogitechGProKeyboardController::SendMode
    (
    unsigned char       zone,
    unsigned char       mode,
    unsigned short      speed,
    unsigned char       red,
    unsigned char       green,
    unsigned char       blue
    )
{
    unsigned char usb_buf[20];

    /*-----------------------------------------------------*\
    | Zero out buffer                                       |
    \*-----------------------------------------------------*/
    memset(usb_buf, 0x00, sizeof(usb_buf));

    /*-----------------------------------------------------*\
    | Set up Lighting Control packet                        |
    \*-----------------------------------------------------*/
    usb_buf[0x00]           = 0x11;
    usb_buf[0x01]           = 0xFF;
    usb_buf[0x02]           = 0x0D;
    usb_buf[0x03]           = 0x3D;
    usb_buf[0x04]           = zone;

    usb_buf[0x05]           = mode;

    usb_buf[0x06]           = red;
    usb_buf[0x07]           = green;
    usb_buf[0x08]           = blue;

    speed = 100 * speed;
    if(mode == LOGITECH_GPRO_MODE_CYCLE)
    {
        usb_buf[0x0B]       = speed >> 8;
        usb_buf[0x0C]       = speed & 0xFF;
        usb_buf[0x0D]       = 0x64;
    }
    else if(mode == LOGITECH_GPRO_MODE_BREATHING)
    {
        usb_buf[0x09]       = speed >> 8;
        usb_buf[0x0A]       = speed & 0xFF;
        usb_buf[0x0C]       = 0x64;
    }

    /*-----------------------------------------------------*\
    | Send packet                                           |
    \*-----------------------------------------------------*/
    hid_write(dev_pkt_0x11, usb_buf, 20);
    hid_read(dev_pkt_0x11, usb_buf, 20);
}
