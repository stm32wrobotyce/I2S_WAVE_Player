/*
 * wave_file.h
 *
 *  Created on: Jul 6, 2021
 *      Author: piotr
 */

#ifndef INC_WAVE_FILE_H_
#define INC_WAVE_FILE_H_

#define WAVE_FILE_HEADER_SIZE	44

#define RIFF_HDR_CONST	0x46464952
#define WAVE_ID_CONST	0x45564157
#define FMT_CONST		0x20746D66
#define CHANNEL_MONO	1

struct __attribute__((packed)) file_header
{
	uint32_t riff_hdr;
	uint32_t file_size;
	uint32_t wave_id;
	uint32_t chunk_marker;
	uint32_t file_format_size;
	uint16_t format_type;
	uint16_t channels;
	uint32_t sample_rate;
	uint32_t bytes_per_sec;
	uint16_t bytes_per_sample;
	uint16_t bits_per_sample;
	uint32_t data_id;
	uint32_t data_size;
};

union wave_file_header
{
    uint8_t bytes[WAVE_FILE_HEADER_SIZE];
    struct file_header wave_file_hdr;
};

#endif /* INC_WAVE_FILE_H_ */
