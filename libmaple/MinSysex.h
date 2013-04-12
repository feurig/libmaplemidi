//
//  lgl_min_sysex.h
//  LibMaple4Midi
//
//  Created by Donald D Davis on 4/11/13.
//  Copyright (c) 2013 Suspect Devices. All rights reserved.
//

#define LEAFLABS_MMA_VENDOR_1   0x7D
#define LEAFLABS_MMA_VENDOR_2   0x1E
#define LEAFLABS_MMA_VENDOR_3   0x4F

#define DEFAULT_MIDI_CHANNEL    0x0A
#define DEFAULT_MIDI_DEVICE     0x0A
#define DEFAULT_MIDI_CABLE      0x00

void LglSysexHandler(uint8 *midiBufferRx,uint32 *rx_offset,uint32 *n_unread_bytes);