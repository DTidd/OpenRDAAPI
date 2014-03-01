
/*
cencode.h - c header for a base64 encoding algorithm

This is part of the libb64 project, and has been placed in the public domain.
For details, see http://sourceforge.net/projects/libb64
*/

#ifndef BASE64_CENCODE_H
#define BASE64_CENCODE_H

typedef enum
{
	step_A, step_B, step_C
} base64_encodestep;

typedef struct
{
	base64_encodestep step;
	char result;
	int stepcount;
} base64_encodestate;

void base64_init_encodestate(base64_encodestate*);

char base64_encode_value(char);

int base64_encode_block(char*, int, char*, base64_encodestate*);

int base64_encode_blockend(char*, base64_encodestate*);

#endif /* BASE64_CENCODE_H */


