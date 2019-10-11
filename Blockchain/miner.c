#include "miner.h"
#include "sha256.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

unsigned char* hash( int block, int nonce, char* data )
{
    SHA256_CTX ctx;
    int length = strlen(data) + 8;
    unsigned char buffer[256] = {0};
    unsigned char* finalHash = malloc( SHA256_BLOCK_SIZE );
    *((int*)((void*)(&buffer[0]))) = block;
    *((int*)((void*)(&buffer[4]))) = nonce;
    sprintf( &(buffer[8]), "%s", data );
    sha256_init( &ctx );
    sha256_update( &ctx, buffer, length );
    sha256_final( &ctx, finalHash );
    return finalHash;
}

static int hasInitialZeros( unsigned char* _hash, int difficulty )
{
    for ( int i = 0; i < difficulty; i++ )
        if ( ( (_hash[i/2] >> (((i+1)%2)*4 ) ) % 16 ) != 0 ) return 0;
    return 1;
}

int mine( int block, char* data, int difficulty )
{
    int nonce = 0;
    unsigned char* currentHash;
    for ( int i = 0; i <= INT32_MAX; i++ )
    {
        currentHash = hash( block, i, data );
        if ( hasInitialZeros( currentHash, difficulty ) )
        {
            free( currentHash );
            nonce = i;
            break;
        }
        free( currentHash );
    }
    return nonce;
}
