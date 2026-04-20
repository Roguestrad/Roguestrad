#ifndef __CRC32_H__
#define __CRC32_H__

/*!
	\brief Initializes a CRC32 checksum value to its initial state

	This function sets the provided CRC32 checksum value to its initial value, which is typically zero or a predefined constant depending on the CRC32 implementation. The function performs a simple
   assignment operation to reset the checksum before processing new data.

	\param crcvalue Reference to the CRC32 value to be initialized
*/
void		 CRC32_InitChecksum( unsigned int& crcvalue );

/*!
	\brief Updates a CRC32 checksum value with the given data and length.

	This function performs an incremental update of a CRC32 checksum value using the standard CRC32 table lookup algorithm. It takes an existing CRC value, applies the CRC32 algorithm to the provided
   data buffer, and updates the checksum value in place. The function processes the data byte by byte, using a lookup table for efficient computation. The input crcvalue is modified to reflect the new
   checksum after processing all bytes in the data buffer.

	\param crcvalue Reference to the CRC32 value to be updated
	\param data Pointer to the data buffer to process
	\param length Number of bytes in the data buffer to process
*/
void		 CRC32_UpdateChecksum( unsigned int& crcvalue, const void* data, int length );

/*!
	\brief Finalizes the CRC32 checksum by applying a bitwise XOR operation with a predefined constant value.

	This function takes an unsigned integer reference representing a CRC32 checksum and applies a final XOR operation with the constant CRC32_XOR_VALUE. This step is typically used to ensure that a
   zero checksum does not occur, which can be important for certain validation or hashing algorithms.

	\param crcvalue Reference to the CRC32 checksum value to be finalized
*/
void		 CRC32_FinishChecksum( unsigned int& crcvalue );

/*!
	\brief Computes the CRC32 checksum for a block of data

	This function calculates a CRC32 checksum for a given block of data by initializing the checksum, updating it with the provided data, and then finalizing the calculation. It is commonly used for
   verifying data integrity. The function handles the complete CRC32 computation process internally, making it a convenient wrapper for checksum calculations.

	\param data pointer to the data block to checksum
	\param length number of bytes in the data block
	\return the computed CRC32 checksum value for the provided data
*/
unsigned int CRC32_BlockChecksum( const void* data, int length );

#endif /* !__CRC32_H__ */
