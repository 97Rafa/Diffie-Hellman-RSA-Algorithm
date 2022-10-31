#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[] ){
    char *outp_file, publicA[256], publicB[256];
    mpz_t p,g,a,b,pbcA,pbcB,scrA,scrB;
    mpz_inits(p,g,a,b,pbcA,pbcB,scrA,scrB,NULL);

    // Here function arguments are being handled
    int opt;
    while ((opt = getopt(argc, argv, "o:p:g:a:b:h")) != -1) {
        switch (opt) {
        case 'o':   // file output target
            outp_file = optarg;
            printf("file: %s\n", outp_file);            
            break;
        case 'p':   // prime number target
            mpz_set_str(p, optarg, 10);
            gmp_printf("prime number: %Zd\n", p);
            break;
        case 'g':   // base number target
            mpz_set_str(g, optarg, 10);
            gmp_printf("base number: %Zd\n", g);
            break;
        case 'a':   // public number a target
            mpz_set_str(a, optarg, 10);
            strcpy(publicA, optarg);
            gmp_printf("public key 1: %Zd\n", a);
            break;
        case 'b':   // public number b target
            mpz_set_str(b, optarg, 10);
            strcpy(publicB, optarg);
            gmp_printf("public key 2: %Zd\n", b);
            break;
        case 'h':
            printf("Options:\n-o path Path to output file\n-p number Prime number\n-g number Primitive Root for previous prime number\n-a number Private key A\n-b number Private key B\n-h This help message\n");
            return 0;
            break;
        default:
            break;
        }
    }

    char scrA_buff[256];
    char scrB_buff[256];

    // Bob and Alice sends their public keys
    mpz_powm(pbcA,g,a,p);   //A = g^a mod p
    mpz_powm(pbcB,g,b,p);   //B = g^b mod p

    // Bob and Alice computes the secret key
    mpz_powm(scrA,pbcB,a,p);    //s = B^a mod p
    mpz_powm(scrB,pbcA,b,p);    //s = A^b mod p

    mpz_get_str(scrA_buff, 10, scrA);
    mpz_get_str(scrB_buff, 10, scrB);

    FILE *fp;
    fp = fopen(outp_file, "w");

    if(strcmp(scrA_buff, scrB_buff) == 0){
        printf("Secret key was successfully created\n");
        fprintf(fp, "<%s>,<%s>,<%s>", publicA, publicB, scrA_buff);    
    }else{
        printf("The procedure went wrong\n");
    }

    fclose(fp);      

    return 0;
}
