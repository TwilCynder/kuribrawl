#include "Port.h"
#include <stdlib.h>

Port::Port(){
    int* buffer = (int*)malloc(CONTROL_STICK_FRAME_BUFFER * 4);

    control_stick_buffer[0] = buffer;
    control_stick_buffer[1] = buffer + 4;
}

void Port::swap_control_stick_buffers(){ 
    int* buffer = control_stick_buffer[1];
    control_stick_buffer[1] = control_stick_buffer[0];
    control_stick_buffer[0] = buffer;
}