/* Copyright (c) 2012-2013 by the author(s)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * =================================================================
 *
 * Driver for the simple message passing hardware.
 *
 * Author(s):
 *   Stefan Wallentowitz <stefan.wallentowitz@tum.de>
 */

#include <or1k-support.h>
#include <optimsoc-sysconfig.h>

#include "include/optimsoc-baremetal.h"
#include <optimsoc.h>

#include <stdlib.h>

#define OPTIMSOC_TRACE_MPSIMPLE_SEND          0x100
#define OPTIMSOC_TRACE_MPSIMPLE_SEND_FINISHED 0x101
#define OPTIMSOC_TRACE_MPSIMPLE_RECV          0x102
#define OPTIMSOC_TRACE_MPSIMPLE_RECV_FINISHED 0x103

// Local buffer for the simple message passing
unsigned int* optimsoc_mp_simple_buffer;

// List of handlers for the classes
void (*cls_handlers[OPTIMSOC_CLASS_NUM])(unsigned int*,int);

void optimsoc_mp_simple_init(void);
void optimsoc_mp_simple_inth(void* arg);

void optimsoc_mp_simple_init(void) {
    // Register interrupt
    or1k_interrupt_handler_add(3,&optimsoc_mp_simple_inth);
    or1k_interrupt_enable(3);

    // Reset class handler
    for (int i=0;i<OPTIMSOC_CLASS_NUM;i++) {
        cls_handlers[i] = 0;
    }

    // Allocate buffer
    optimsoc_mp_simple_buffer = malloc(_optimsoc_noc_maxpacketsize*4);
}

void optimsoc_mp_simple_addhandler(unsigned int class,
        void (*hnd)(unsigned int*,int)) {
    cls_handlers[class] = hnd;
}

void optimsoc_mp_simple_inth(void* arg) {
    while (1) {
        // Store message in buffer
        // Get size
        int size = REG32(OPTIMSOC_MPSIMPLE_RECV);

        if (size==0) {
            // There are no further messages in the buffer
            break;
        } else if (_optimsoc_noc_maxpacketsize<size) {
            // Abort and drop if message cannot be stored
//            printf("FATAL: not sufficent buffer space. Drop packet\n");
            for (int i=0;i<size;i++) {
                REG32(OPTIMSOC_MPSIMPLE_RECV);
            }
        } else {
            for (int i=0;i<size;i++) {
                optimsoc_mp_simple_buffer[i] = REG32(OPTIMSOC_MPSIMPLE_RECV);
            }
        }

        // Extract class
        int class = (optimsoc_mp_simple_buffer[0] >> OPTIMSOC_CLASS_LSB) // Shift to position
    		                & ((1<<(OPTIMSOC_CLASS_MSB-OPTIMSOC_CLASS_LSB+1))-1); // and mask other remain

        // Call respective class handler
        if (cls_handlers[class] == 0) {
            // No handler registered, packet gets lost
            //printf("Packet of unknown class (%d) received. Drop.\n",class);
            continue;
        }

        OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_RECV,(optimsoc_mp_simple_buffer[0]>>OPTIMSOC_SRC_LSB)&0x1f);
        OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_RECV,class);
        OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_RECV,size);
        cls_handlers[class](optimsoc_mp_simple_buffer,size);
        OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_RECV_FINISHED,0);
    }
}

void optimsoc_mp_simple_send(unsigned int size, uint32_t *buf) {
    OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_SEND,buf[0]>>OPTIMSOC_DEST_LSB);
    OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_SEND,size);
    OPTIMSOC_TRACE(OPTIMSOC_TRACE_MPSIMPLE_SEND,buf);

    int restore = optimsoc_critical_begin();
    REG32(OPTIMSOC_MPSIMPLE_SEND) = size;
    for (int i=0;i<size;i++) {
        REG32(OPTIMSOC_MPSIMPLE_SEND) = buf[i];
    }
    optimsoc_critical_end(restore);
}

void optimsoc_send_alive_message() {
    if (_optimsoc_has_hostlink) {
        unsigned int buffer;
        buffer = (_optimsoc_hostlink << OPTIMSOC_DEST_LSB) |
                (1 << OPTIMSOC_CLASS_LSB) |
                (optimsoc_get_tileid() << OPTIMSOC_SRC_LSB);
    }
}


