// Copyright 2011 Juri Glass, Mathias Runge, Nadim El Sayed 
// DAI-Labor, TU-Berlin
// 
// This file is part of libSML.
// 
// libSML is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// libSML is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with libSML.  If not, see <http://www.gnu.org/licenses/>.


#include <string.h>
#include <sml/sml_shared.h>
#include <stdio.h>

int sml_buf_get_next_length(sml_buffer *buf) {
	int length = 0;
	unsigned char byte = mc_sml_buf_get_current_byte(buf);
	int list = ((byte & SML_TYPE_FIELD) == SML_TYPE_LIST) ? 0 : -1;
	
	for (;buf->cursor < buf->buffer_len;) {
		byte = mc_sml_buf_get_current_byte(buf);
		length <<= 4;
		length |= (byte & SML_LENGTH_FIELD);
		
		if ((byte & SML_ANOTHER_TL) != SML_ANOTHER_TL) {
			break;
		}
        mc_sml_buf_update_read_bytes(buf, 1);
        if(list) {
            list += -1;
        }
	}
	mc_sml_buf_update_read_bytes(buf, 1);
	return length + list;
}

void mc_sml_set_type_and_length(sml_buffer *buf, unsigned int type, unsigned int l) {
    // set the type
    buf->buffer[buf->cursor] |= type;
    
    if (type != SML_TYPE_LIST) {
        l++;
    }
    
    if (l > SML_LENGTH_FIELD) {
        // how much shifts are necessary
        int mask_pos = (sizeof(unsigned int) * 2) - 1;  
        
        // the 4 most significant bits of l
        unsigned int mask = 0xF0 << (8 * (sizeof(unsigned int) - 1));
        
        // select the 4 most significant bits with a bit set
        while (!(mask & l)) {
            mask >>= 4;
            mask_pos--;
        }
        
        // copy the bits to the buffer
        while (mask > SML_LENGTH_FIELD) {
            buf->buffer[buf->cursor] |= SML_ANOTHER_TL;
            buf->buffer[buf->cursor] |= ((mask & l) >> (4 * mask_pos));
            mask >>= 4;
            mask_pos--;
            buf->cursor++;
        }
    }
    buf->buffer[buf->cursor] |= (l & SML_LENGTH_FIELD);
    buf->cursor++;
}

int mc_sml_buf_has_errors(sml_buffer *buf) {
	return buf->error != 0;
}

int mc_sml_buf_get_current_type(sml_buffer *buf) {
	return (buf->buffer[buf->cursor] & SML_TYPE_FIELD);
}

unsigned char mc_sml_buf_get_current_byte(sml_buffer *buf) {
	return buf->buffer[buf->cursor];
}

unsigned char *mc_sml_buf_get_current_buf(sml_buffer *buf) {
	return &(buf->buffer[buf->cursor]);
}

void mc_sml_buf_update_read_bytes(sml_buffer *buf, int bytes) {
	buf->cursor += bytes;
}

sml_buffer *sml_buffer_init(size_t length) {
    sml_buffer *buf = (sml_buffer *) malloc(sizeof(sml_buffer));
    memset(buf, 0, sizeof(sml_buffer));
    buf->buffer = (unsigned char *) malloc(length);
    buf->buffer_len = length;
    memset(buf->buffer, 0, buf->buffer_len);
    return buf;
}

void mc_sml_optional_write(sml_buffer *buf) {
    buf->buffer[buf->cursor] = SML_OPTIONAL_SKIPPED;
    buf->cursor++;
}

void sml_buffer_free(sml_buffer *buf) {
    if (buf) {
        if (buf->buffer)
            free(buf->buffer);
        if (buf->error_msg)
            free(buf->error_msg);
        free(buf);
    }
}

int mc_sml_is_optional_skipped(sml_buffer *buf) {
	if (mc_sml_buf_get_current_byte(buf) == SML_OPTIONAL_SKIPPED) {
		mc_sml_buf_update_read_bytes(buf, 1);
		return 1;
	}
	return 0;
}

void hexdump(unsigned char *buffer, size_t buffer_len) {
	int i;
	for (i = 0; i < buffer_len; i++) {
		printf("%02X ", (unsigned char) buffer[i]);
		if ((i + 1) % 8 == 0) {
			printf("\n");
		}
	}
	printf("\n");
}