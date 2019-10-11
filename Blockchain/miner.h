#ifndef miner_h
#define miner_h

unsigned char* hash( int block, int nonce, char* data );
int mine( int block, char* data, int difficulty );

#endif // miner_h
