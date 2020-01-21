/*
   Copyright 2020 Raphael Beck

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.
*/

/**
 *  @file claim.h
 *  @author Raphael Beck
 *  @brief JWT claims as described in https://auth0.com/docs/tokens/concepts/jwt-claims
 */

#ifndef L8W8JWT_CLAIM_H
#define L8W8JWT_CLAIM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include "chillbuff.h"

#define L8W8JWT_CLAIM_TYPE_STRING 0
#define L8W8JWT_CLAIM_TYPE_INTEGER 1
#define L8W8JWT_CLAIM_TYPE_NUMBER 2
#define L8W8JWT_CLAIM_TYPE_BOOLEAN 3
#define L8W8JWT_CLAIM_TYPE_NULL 4
#define L8W8JWT_CLAIM_TYPE_ARRAY 5
#define L8W8JWT_CLAIM_TYPE_OBJECT 6
#define L8W8JWT_CLAIM_TYPE_OTHER 7

/**
 * Struct containing a jwt claim key-value pair.<p>
 * If allocated on the heap by the decode function,
 * remember to call <code>l8w8jwt_claims_free()</code> on it once you're done using it.
 */
struct l8w8jwt_claim
{
    /**
     * The token claim key (e.g. "iss", "iat", "sub", etc...). <p>
     * NUL-terminated C-string!
     */
    char* key;

    /**
     * key string length. <p>
     * Set this to <code>0</code> if you want to make the encoder use <code>strlen(key)</code> instead.
     */
    size_t key_length;

    /**
     * The claim's value as a NUL-terminated C-string.
     */
    char* value;

    /**
     * value string length. <p>
     * Set this to <code>0</code> if you want to make the encoder use <code>strlen(value)</code> instead.
     */
    size_t value_length;

    /**
     * Claim type. <p>
     * 0 = string, 1 = integer, 2 = number, 3 = boolean, 4 = null, 5 = array, 6 = object, 7 = other.
     */
    int type;
};

/**
 * Frees a heap-allocated array of <code>l8w8jwt_claim</code>s.
 * @param claims The claims to free.
 * @param claims_count The size of the passed claims array.
 */
void l8w8jwt_free_claims(struct l8w8jwt_claim* claims, size_t claims_count);

/**
 * Writes a bunch of JWT claims into a chillbuff stringbuilder. <p>
 * Curly braces and trailing commas won't be written; only the "key":"value" pairs!
 * @param stringbuilder The buffer into which to write the claims.
 * @param claims The l8w8jwt_claim array of claims to write.
 * @param claims_count The claims array size.
 * @return Return code as specified inside retcodes.h
 */
int l8w8jwt_write_claims(chillbuff* stringbuilder, struct l8w8jwt_claim* claims, size_t claims_count);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // L8W8JWT_CLAIM_H
