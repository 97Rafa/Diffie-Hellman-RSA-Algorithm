#include <gmp.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>



bool isPrime(int n){
    for(int i = 2; i < n; i++)  //Check for factors
    {
        if(n % i == 0)
            return false;
    }
    return true;
}

void key_generation(){
     mpz_t p, q, n, p_minus, q_minus, lambda, e, gcd, modulo, d;
    mpz_inits(p, q, n, p_minus, q_minus, lambda, e, gcd, modulo, d, NULL);
    

    // =======  Step 1  =======
    int p_value, q_value;
    printf("Please insert a prime p number:\n");
    scanf("%d", &p_value);

    printf("Please insert a prime p number:\n");
    scanf("%d", &q_value);

    // =======  Step 2 & 3 =======
    if(isPrime(p_value) && isPrime(q_value)){
        // printf("einai monoi\n");
        mpz_set_ui(p, p_value);
        mpz_set_ui(q, q_value);

        mpz_mul(n,p,q);
    }else{
        printf("The numbers you typed were NOT primes\n");
        return;
    }

    // =======  Step 4  =======
    mpz_sub_ui(p_minus,p,1);
    mpz_sub_ui(q_minus,q,1);

    mpz_mul(lambda, p_minus, q_minus);

    printf("The lambda is: %s = %s * %s\n", 
            mpz_get_str(NULL,0,lambda), mpz_get_str(NULL,0,p_minus), mpz_get_str(NULL,0,q_minus));

    // =======  Step 5  =======
    for (size_t i = 2; i < mpz_get_ui(lambda); i++)
    {
        
        mpz_set_ui(e, i);
        mpz_gcd(gcd, e, lambda);
        mpz_mod(modulo, e, lambda);
        
        // (e % lambda(n) != 0) AND (gcd(e, lambda) == 1)
        if ((mpz_get_ui(modulo) != 0) && (mpz_get_ui(gcd) == 1) && isPrime(mpz_get_ui(e)))
        {
            break;
        }
        
    }   
    
    gmp_printf("\n\nYour 'e' number is %Zd\n", e);

    // =======  Step 6  =======
    mpz_invert(d, e, lambda);

    // =======  Step 7 & 8  =======
    FILE *fp;
    
    size_t final_e, final_d, final_n;

    final_d = mpz_get_ui(d);
    final_e = mpz_get_ui(e);
    final_n = mpz_get_ui(n);

    
    fp = fopen("public.key", "w");
    fwrite(&final_n, sizeof(size_t), 1, fp);
    fwrite(&final_d, sizeof(size_t), 1, fp);
    fclose(fp);

    fp = fopen("private.key", "w");
    fwrite(&final_n, sizeof(size_t), 1, fp);
    fwrite(&final_e, sizeof(size_t), 1, fp);    
    fclose(fp);

}

void encryption(char *key_f, char *input_f, char *output_f){
    FILE *fp;
    size_t temp_n, temp_d;
    mpz_t n, d, plain_text_c, ciphertext;
    mpz_inits(n, d, plain_text_c, ciphertext, NULL);
    int len;
    int i = 0;
    fp = fopen(key_f, "r");

    fread(&temp_n, sizeof(size_t), 1, fp);
    fread(&temp_d, sizeof(size_t), 1, fp);

    fclose(fp);

    mpz_set_ui(n, temp_n);
    mpz_set_ui(d, temp_d);

    fp = fopen(input_f, "r");

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char textToEncr[len];

    do{
        textToEncr[i] = fgetc(fp);
        i++;
    }while(i<len);

    textToEncr[len] = '\0';
    fclose(fp);

    fp = fopen(output_f, "w");
    i=0;
    size_t temp_char, temp_cipher;
    do
    {
        temp_char = (size_t)textToEncr[i];
        mpz_set_ui(plain_text_c, temp_char);
        
        // ciphertext = plaintext^d  mod n
        mpz_powm(ciphertext, plain_text_c, d, n);
        temp_cipher = mpz_get_ui(ciphertext);
        fwrite(&temp_cipher, sizeof(size_t), 1, fp);
        i++;
    } while (i<len);

    fclose(fp);

}

void decrypt(char *key_f, char *input_f, char *output_f){
    FILE *fp;
    size_t temp_n, temp_e;
    mpz_t n, e, plain_text_c, ciphertext;
    mpz_inits(n, e, plain_text_c, ciphertext, NULL);
    int len=0;
    int i = 0;
    fp = fopen(key_f, "r");

    fread(&temp_n, sizeof(size_t), 1, fp);
    fread(&temp_e, sizeof(size_t), 1, fp);

    fclose(fp);

    mpz_set_ui(n, temp_n);
    mpz_set_ui(e, temp_e);

    fp = fopen(input_f, "r");

    fseek(fp, 0, SEEK_END);
    len = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    size_t textToDecr[len],temp_cipher, temp_decrypted;

    fread(textToDecr, sizeof(size_t), len, fp);
    fclose(fp);

    char temp_plain_text[len/sizeof(size_t)];

    fp = fopen(output_f, "w");
    i=0;
    do
    {
        mpz_set_ui(ciphertext, textToDecr[i]);
        
        // plaintext = ciphertext^e  mod n
        mpz_powm(plain_text_c, ciphertext, e, n);
        temp_plain_text[i] = mpz_get_ui(plain_text_c);
        i++;
    } while (i<len/(sizeof(size_t)));

    temp_plain_text[len/(sizeof(size_t))] = '\0';

    
    fwrite(&temp_plain_text, sizeof(char), len/sizeof(size_t), fp);


    fclose(fp);

}

int main(int argc, char *argv[] ){

    // Here function arguments are being handled
    int opt;
    char *input_file, *output_file, *key_file;
    while ((opt = getopt(argc, argv, "i:o:k:gdeh")) != -1) {
        switch (opt) {
        case 'i':   // Path to the input file
            input_file = optarg;
            break;
        case 'o':   // Path to the output file
            output_file = optarg;
            break;
        case 'k':   // Path to the key file
            key_file = optarg;
            break;
        case 'g':   // Perform RSA key-pair generation
            key_generation();
            break;
        case 'd':   // Decrypt input and store results to output
            decrypt(key_file, input_file, output_file);
            break;
        case 'e':   // Encrypt input and store results to output
            encryption(key_file, input_file, output_file);
            break;
        case 'h':
            printf("Options:\n-i path Path to the input file\n-o path Path to the output file\n-k path Path to the key file\n-g Perform RSA key-pair generation\n-d Decrypt input and store results to output\n-e Encrypt input and store results to output\n-h This help message\n");
            return 0;
            break;
        default:
            break;
        }
    }

    return 0;
}