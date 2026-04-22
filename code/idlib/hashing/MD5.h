#ifndef __MD5_H__
#define __MD5_H__

/*
===============================================================================

	Calculates a checksum for a block of data
	using the MD5 message-digest algorithm.

===============================================================================
*/
struct MD5_CTX {
	unsigned int  state[4];
	unsigned int  bits[2];
	unsigned char in[64];
};

//! Initializes the MD5 context structure with default values.
void		 MD5_Init( MD5_CTX* ctx );

//! Updates the MD5 context with a new message block.
void		 MD5_Update( MD5_CTX* context, unsigned char const* input, size_t inputLen );

//! Computes the final MD5 hash digest and zeroizes the context
void		 MD5_Final( MD5_CTX* context, unsigned char digest[16] );

//! Computes a 32-bit checksum from the given data using MD5 algorithm
unsigned int MD5_BlockChecksum( const void* data, size_t length );

#endif /* !__MD5_H__ */
