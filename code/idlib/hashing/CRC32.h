#ifndef __CRC32_H__
#define __CRC32_H__

//! Initializes the CRC32 checksum value to its initial state.
void		 CRC32_InitChecksum( unsigned int& crcvalue );

//! Updates a CRC32 checksum value with the given data buffer and length.
void		 CRC32_UpdateChecksum( unsigned int& crcvalue, const void* data, int length );

//! Finalizes the CRC32 checksum by applying a bitwise XOR operation with a predefined constant value to the provided checksum value.
void		 CRC32_FinishChecksum( unsigned int& crcvalue );

//! Computes the CRC32 checksum for a block of data.
unsigned int CRC32_BlockChecksum( const void* data, int length );

#endif /* !__CRC32_H__ */
