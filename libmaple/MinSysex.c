//
//  MinSysex.c
//  LibMaple4Midi
//
//  Created by Donald D Davis on 4/11/13.
//  Copyright (c) 2013 Suspect Devices. All rights reserved.
//
/*
 0xF0  SysEx
 0x7E  Non-Realtime
 0x7F  The SysEx channel. Could be from 0x00 to 0x7F.
 Here we set it to "disregard channel".
 0x06  Sub-ID -- General Information
 0x01  Sub-ID2 -- Identity Request
 0xF7  End of SysEx
*/ 

#include <libmaple/usb_midi_device.h>
#include <MinSysex.h>
#define MAX_SYSEX_SIZE 256
typedef enum  {NOT_IN_SYSEX=0,IN_SYSEX,COULD_BE_MY_SYSEX,YUP_ITS_MY_SYSEX,ITS_NOT_MY_SYSEX,I_GOT_MY_SYSEX} sysexStates;
volatile uint8 sysexBuffer[MAX_SYSEX_SIZE];
volatile sysexStates sysexState;
volatile int sysexFinger=0;
void dealWithItQuickly(){
    ;//turn the led on?
}

void LglSysexHandler(uint8 *midiBufferRx,uint32 *rx_offset,uint32 *n_unread_bytes) {
    MIDI_EVENT_PACKET_t * midiPackets = (MIDI_EVENT_PACKET_t *) (midiBufferRx+(*rx_offset));
    uint8 nPackets=(*n_unread_bytes)-(*rx_offset);
    int cPacket;
    /********************************* ACHTUNG! ignores usbmidi cable ********************************/
    MIDI_EVENT_PACKET_t *packet;
    for (cPacket=0;cPacket<nPackets;cPacket++){
        packet=midiPackets+cPacket;
        if (!CIN_IS_SYSEX(packet->cin)) {
            continue;
        } // else {
        switch (packet->cin) {
            case CIN_SYSEX:
                switch (sysexState) {
                    case NOT_IN_SYSEX : // new sysex.
                        sysexFinger=0;
                        if (packet->midi0 == MIDIv1_SYSEX_START) {
                            if (packet->midi1==MIDIv1_UNIVERSAL_REALTIME_ID
                                ||packet->midi1==MIDIv1_UNIVERSAL_NON_REALTIME_ID) {
                                if ((packet->midi2==myMidiChannel)
                                    ||(packet->midi2==MIDIv1_UNIVERSAL_ALL_CHANNELS)
                                    ) {
                                    sysexState=YUP_ITS_MY_SYSEX;
                                    sysexBuffer[sysexFinger++]=MIDIv1_SYSEX_START;
                                    sysexBuffer[sysexFinger++]=packet->midi1;
                                    sysexBuffer[sysexFinger++]=packet->midi2;
                                    break;
                                }
                            } else if ((packet->midi1==myMidiID[0])
                                       && (packet->midi2==myMidiID[1])
                                       ){
                                sysexState=COULD_BE_MY_SYSEX;
                                sysexBuffer[sysexFinger++]=MIDIv1_SYSEX_START;
                                sysexBuffer[sysexFinger++]=packet->midi1;
                                sysexBuffer[sysexFinger++]=packet->midi2;
                                break;
                            }
                        }
                        break;
                        
                    case COULD_BE_MY_SYSEX:
                        if (packet->midi0==myMidiID[2]) {
                            sysexState=YUP_ITS_MY_SYSEX;
                            sysexBuffer[sysexFinger++]=packet->midi0;
                            sysexBuffer[sysexFinger++]=packet->midi1;
                            sysexBuffer[sysexFinger++]=packet->midi2;
                        } else {
                            sysexState=ITS_NOT_MY_SYSEX;
                            sysexFinger=0;
                        }
                        break;
                    default:
                        break;
                        
                }
                
                break;
            case CIN_SYSEX_ENDS_IN_1:
            case CIN_SYSEX_ENDS_IN_2:
            case CIN_SYSEX_ENDS_IN_3:
                sysexBuffer[sysexFinger++]=packet->midi0;
                sysexBuffer[sysexFinger++]=packet->midi1;
                sysexBuffer[sysexFinger++]=packet->midi2;
                // sysexState=I_GOT_MY_SYSEX;
                // flag it or something for retrieval later.
                dealWithItQuickly();
                sysexFinger=0;
                sysexState=NOT_IN_SYSEX;

                break;
            default:
                return;
        }
        //}
    

    
    }
    // its our sysex and we will cry if we want to
    return;
}