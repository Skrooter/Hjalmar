/*
 * audio_interface.h
 *
 *  Created on: Nov 26, 2017
 *      Author: jaxc
 */

#ifndef INC_AUDIO_INTERFACE_H_
#define INC_AUDIO_INTERFACE_H_

#define AUDIO_BUFFER_SIZE 32

void init_audio_output(void);
//void add_to_audio_buffer(int16_t *audio_buffer, uint32_t buf_len);
void send_audio(void);


#endif /* INC_AUDIO_INTERFACE_H_ */
