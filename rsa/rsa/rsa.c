/*
* gcc -o rsa-encrypt rsa-encrypt.c -lcrypto
*/
#include <openssl/rsa.h>
#include <openssl/err.h>
#include <strings.h>

#define MODULUS           "C8FBCF21"
#define PUBLIC_EXPONENT   RSA_F4
#define PRIVATE_EXPONENT  "97B55D7D"

int encrypt()
{
    int ret, flen;
    BIGNUM *bnn, *bne, *bnd;
    unsigned char *in = "abc";
    unsigned char *out;

    bnn = BN_new();
    bne = BN_new();
    bnd = BN_new();
    BN_hex2bn(&bnn, MODULUS);
    BN_set_word(bne, PUBLIC_EXPONENT);
    BN_hex2bn(&bnd, PRIVATE_EXPONENT);

    RSA *r = RSA_new();
    r->n = bnn;
    r->e = bne;
    r->d = bnd;
    RSA_print_fp(stdout, r, 5);

    flen = RSA_size(r);// - 11;
    out = (char *)malloc(flen);
    bzero(out, flen);

    printf("Begin encrypt... ");
    ret = RSA_private_encrypt(flen, in, out, r,  RSA_NO_PADDING);
    if (ret < 0)
    {
        printf("Encrypt failed! ");
        return 1;
    }

    printf("Size:%d ", ret);
    printf("ClearText:%s ", in);
    printf("CipherText(Hex):");
    int i;
    for (i=0; i<ret; i++)
    {
        printf("0x%02x, ", *out);
        out++;
    }
    printf("\n\n");

    //free(out);
    RSA_free(r);
    return 0;
}


int decrypt()
{
    int ret, flen;
    BIGNUM *bnn, *bne;
    unsigned char in[] = {0x98, 0x79, 0xb2, 0x76};
    unsigned char *out;

    bnn = BN_new();
    bne = BN_new();
    BN_hex2bn(&bnn, MODULUS);
    BN_set_word(bne, PUBLIC_EXPONENT);

    RSA *r = RSA_new();
    r->n = bnn;
    r->e = bne;
    RSA_print_fp(stdout, r, 5);

    flen = RSA_size(r);
    out = (unsigned char *)malloc(flen);
    bzero(out, flen);

    printf("Begin decrypt... ");
    ret = RSA_public_decrypt(sizeof(in), in, out, r, RSA_NO_PADDING);
    if (ret < 0)
    {
        printf("Decrypt failed! ");
        return 1;
    }

    printf("Size:%d ", ret);
    printf("ClearText:%s ", out);

    printf("\n\n");

    free(out);
    RSA_free(r);
    return 0;
}

int main()
{
    encrypt();
    decrypt();
}
