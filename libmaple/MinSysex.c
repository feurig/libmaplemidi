//
//  MinSysex.c
//  LibMaple4Midi
//
//  Created by Donald D Davis on 4/11/13.
//  Copyright (c) 2013 Suspect Devices. All rights reserved.
//

#include <libmaple/usb_midi_device.h>
#include <MinSysex.h>

void LglSysexHandler(uint8 *midiBufferRx,uint32 *rx_offset,uint32 *n_unread_bytes) {
    // its our sysex and we will cry if we want to
    return;
}