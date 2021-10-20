/*
 * wave_player.h
 *
 *  Created on: 7 lip 2021
 *      Author: piotr
 */

#ifndef INC_WAVE_PLAYER_H_
#define INC_WAVE_PLAYER_H_

#define FIRST_HALF_OF_BUFFER		1
#define SECOND_HALF_OF_BUFFER		2

#define AUDIO_BUFFER_SIZE			512
#define BYTES_IN_AUDIO_BUFFER_SIZE	AUDIO_BUFFER_SIZE*2

struct wave_player_s
{
	SAI_HandleTypeDef *hsai;

    uint8_t *data_pointer;
	uint8_t buffer[AUDIO_BUFFER_SIZE];
	volatile uint32_t byte_counter;
	union wave_file_header file_hdr;
};

void wave_player_init(SAI_HandleTypeDef *_hsai);
void wave_player_start(uint8_t *file);
int8_t wave_player_read_header(uint8_t *file);
void wave_player_set_sample_rate(uint32_t sample_rate);
void wave_player_prepare_first_buffer(void);
void wave_player_prepare_half_buffer(uint8_t half_number);
void wave_player_prepare_data(uint32_t start_address, uint32_t end_address);

#endif /* INC_WAVE_PLAYER_H_ */
