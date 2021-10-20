/*
 * wave_file.c
 *
 *  Created on: Jul 6, 2021
 *      Author: piotr
 */

#include "main.h"

struct wave_player_s wave_player;
extern uint8_t audio_file[];

void wave_player_init(SAI_HandleTypeDef *_hsai)
{
	wave_player.hsai = _hsai;
}

void wave_player_start(uint8_t *file)
{
	int8_t status;

	status = wave_player_read_header(file);

	if(ERROR == status)
	{
		return;
	}

	wave_player_set_sample_rate(wave_player.file_hdr.wave_file_hdr.sample_rate);
	wave_player_prepare_first_buffer();

	HAL_SAI_Transmit_DMA(wave_player.hsai, (uint8_t *)wave_player.buffer, AUDIO_BUFFER_SIZE/2);
}

int8_t wave_player_read_header(uint8_t *file)
{
	uint32_t i;

	wave_player.data_pointer = file;

	for(i = 0; i < WAVE_FILE_HEADER_SIZE; i++)
	{
		wave_player.file_hdr.bytes[i] = *(wave_player.data_pointer+i);
	}

	if(RIFF_HDR_CONST != wave_player.file_hdr.wave_file_hdr.riff_hdr)
	{
		return ERROR;
	}

	if(WAVE_ID_CONST != wave_player.file_hdr.wave_file_hdr.wave_id)
	{
		return ERROR;
	}

	if(FMT_CONST != wave_player.file_hdr.wave_file_hdr.chunk_marker)
	{
		return ERROR;
	}

	if(CHANNEL_MONO != wave_player.file_hdr.wave_file_hdr.channels)
	{
		return ERROR;
	}

	wave_player.byte_counter = WAVE_FILE_HEADER_SIZE;

	return SUCCESS;
}

void wave_player_set_sample_rate(uint32_t sample_rate)
{
	wave_player.hsai->Init.AudioFrequency = sample_rate;

	HAL_SAI_InitProtocol(wave_player.hsai, SAI_I2S_STANDARD, SAI_PROTOCOL_DATASIZE_16BIT, 2);
}

void wave_player_prepare_first_buffer(void)
{
	wave_player_prepare_data(0, AUDIO_BUFFER_SIZE);
}

void wave_player_prepare_half_buffer(uint8_t half_number)
{
	if(FIRST_HALF_OF_BUFFER == half_number)
	{
		wave_player_prepare_data(0, AUDIO_BUFFER_SIZE/2);
	}
	else if(SECOND_HALF_OF_BUFFER == half_number)
	{
		HAL_SAI_Transmit_DMA(wave_player.hsai, (uint8_t *)wave_player.buffer, AUDIO_BUFFER_SIZE/2);
		wave_player_prepare_data(AUDIO_BUFFER_SIZE/2, AUDIO_BUFFER_SIZE);
	}
}

void wave_player_prepare_data(uint32_t start_address, uint32_t end_address)
{
	uint8_t audio_sample;

	for(int i = start_address; i < end_address; i++)
	{
		audio_sample = *(wave_player.data_pointer+wave_player.byte_counter);

		wave_player.buffer[i] = audio_sample;
		wave_player.byte_counter++;

		if(wave_player.byte_counter >= wave_player.file_hdr.wave_file_hdr.data_size)
		{
			HAL_SAI_DMAPause(wave_player.hsai);
			return;
		}
	}
}
