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

/*!
	\brief Initializes the MD5 context structure with default values.

	This function sets up the initial state values for the MD5 context used in the MD5 hash computation. It initializes the four 32-bit state variables and the bit count variables to their standard
   MD5 initial values. The state variables are set to the constants defined in the MD5 algorithm specification, and the bit count is reset to zero.

	\param ctx Pointer to the MD5 context structure to be initialized
*/
void		 MD5_Init( MD5_CTX* ctx );

/*!
	\brief Updates the MD5 context with a new chunk of input data.

	This function processes a chunk of input data and updates the internal MD5 context. It handles the incremental processing of data by managing the buffer and performing MD5 transformation as
   needed. The function correctly accounts for the bit count and ensures proper handling of data that spans multiple 64-byte blocks. It is designed to be called multiple times with successive chunks
   of data to compute the MD5 hash of a complete message.

	\param context Pointer to the MD5 context structure to update
	\param input Pointer to the input data to process
	\param inputLen Number of bytes of input data to process
*/
void		 MD5_Update( MD5_CTX* context, unsigned char const* input, size_t inputLen );

/*!
	\brief Finalizes the MD5 hash computation and stores the resulting digest

	This function completes the MD5 hash computation by adding padding to the input data and appending the bit length. It then stores the final 128-bit hash value in the provided digest buffer. The
   context structure is zeroized after the computation to prevent sensitive information from remaining in memory. The function assumes that MD5_Update has been called previously to process the input
   data.

	\param context Pointer to the MD5 context structure containing the state and bit count
	\param digest Buffer to store the resulting 16-byte MD5 digest
*/
void		 MD5_Final( MD5_CTX* context, unsigned char digest[16] );

/*!
	\brief Computes a checksum value for a memory block using MD5 hashing and a custom XOR combination of digest bytes.

	This function takes a memory block and its length, computes the MD5 hash of the data, and then combines specific bytes from the hash digest into a single unsigned integer value. The combination is
   done through a XOR operation of four 32-bit values constructed from 4 bytes each from the digest. The function is endian-safe as it manually extracts and shifts bytes without relying on
   byte-swapping utilities.

	\param data Pointer to the memory block to calculate the checksum for
	\param length Size of the memory block in bytes
	\return A 32-bit unsigned integer value representing the computed checksum
*/
unsigned int MD5_BlockChecksum( const void* data, size_t length );

#endif /* !__MD5_H__ */
