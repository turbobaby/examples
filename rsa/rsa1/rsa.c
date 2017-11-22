#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<openssl/rsa.h>
#include<openssl/pem.h>
#include<openssl/err.h>
#include <stdlib.h>
#include <time.h>

#define OPENSSLKEY "test.key"
#define PUBLICKEY  "test_pub.key"
#define BUFFSIZE 1024

struct data
{
    unsigned char *dptr;
    int            dlen;
};

int my_encrypt(struct data *src, struct data *enc, char *path_key);//加密
int my_decrypt(struct data *enc, struct data *dec, char *path_key);//解密

int main(void) {
    struct data src, enc, dec;

    unsigned char *source = "i like dancing ! abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz abcdefghijklmnopqrstuvwxyz";

    unsigned char *ptr_en, *ptr_de, *tmp;
    int i;
    int rc;

    srand(time(NULL));

    src.dptr = source;
    src.dlen = strlen(source);

    printf("source is    :%s\n", src.dptr);
    rc = my_encrypt(&src, &enc, PUBLICKEY);

    tmp = enc.dptr;
    for (i = 0; i <= enc.dlen;i++) {
        printf("%02x ", *tmp++);
    }
    printf("\n");

    rc = my_decrypt(&enc, &dec, OPENSSLKEY);
    printf("after decrypt:%s\n", dec.dptr);
    if (enc.dptr != NULL) {
        free(enc.dptr);
    }
    if( dec.dptr != NULL ) {
        free(dec.dptr);
    }
    return 0;
}

int my_encrypt(struct data *src, struct data *enc, char *path_key) {//加密
    RSA *p_rsa;
    FILE *file;
    int rsa_len;

    if ((file = fopen(path_key, "r")) == NULL) {
        perror("open key file error");
        return -1;    
    }

    if((p_rsa = PEM_read_RSA_PUBKEY(file, NULL, NULL, NULL)) == NULL) {
        ERR_print_errors_fp(stdout);
        return -1;
    }

    rsa_len= RSA_size(p_rsa);
    printf("enc rsa_len = %d\n", rsa_len);
    enc->dptr = (unsigned char *)malloc(rsa_len);
    memset(enc->dptr, 0, rsa_len);
    enc->dlen = rsa_len;

    if (RSA_public_encrypt(rsa_len, (unsigned char *)src->dptr, (unsigned char*)enc->dptr, p_rsa, RSA_NO_PADDING) < 0) {
        return -1;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 0;
}

int my_decrypt(struct data *enc, struct data *dec, char *path_key) {//解密
    RSA *p_rsa;
    FILE *file;
    int rsa_len;

    if ((file = fopen(path_key,"r"))==NULL){
        perror("open key file error");
        return -1;
    }

    if ((p_rsa = PEM_read_RSAPrivateKey(file, NULL, NULL, NULL)) == NULL) {
        ERR_print_errors_fp(stdout);
        return -1;
    }
    rsa_len = RSA_size(p_rsa);
    printf("dec rsa_len = %d\n", rsa_len);
    dec->dptr=(unsigned char *)malloc(rsa_len);
    memset(dec->dptr, 0, rsa_len);
    dec->dlen = rsa_len;

    if (RSA_private_decrypt(rsa_len, (unsigned char *)enc->dptr, (unsigned char*)dec->dptr, p_rsa, RSA_NO_PADDING) < 0) {
        return -1;
    }
    RSA_free(p_rsa);
    fclose(file);
    return 0;
}
