#ifndef SECRETS_H
#define SECRETS_H
#include "ecdsa.h"

#define ECDSA_CURVE ecdsa_secp256r1
#define SIZE_ECDSA_CURVE 32
#define SIZE_ECDSA_PRIVATE SIZE_ECDSA_CURVE
#define SIZE_ECDSA_PUBLIC 2*SIZE_ECDSA_CURVE

void crypt_keygen();
    


#endif /* SECRETS_H */

