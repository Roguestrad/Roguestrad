#ifndef __MD4_H__
#define __MD4_H__

//! Computes a checksum from the given data using the MD4 hash algorithm and XORs the hash components.
unsigned int MD4_BlockChecksum( const void* data, int length );

#endif /* !__MD4_H__ */
