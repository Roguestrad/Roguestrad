#ifndef __MD4_H__
#define __MD4_H__

/*!
	\brief Computes a checksum for a data block using the MD4 hash algorithm and XORs the hash components.

	This function calculates the MD4 hash of the provided data block and combines the resulting hash components using XOR operation. It is used for generating checksums, particularly in the context of
   file handling and data integrity verification. The function initializes an MD4 context, updates it with the input data, finalizes the hash calculation, and then XORs the four 32-bit hash values to
   produce a single 32-bit checksum value.

	\param data Pointer to the data block to checksum
	\param length Length of the data block in bytes
	\return A 32-bit unsigned integer representing the computed checksum
*/
unsigned int MD4_BlockChecksum( const void* data, int length );

#endif /* !__MD4_H__ */
