# Diffie-Hellman-RSA-Algorithm

======  Diffie-Hellman Key Exchange Tool   ======
A C-Language implemented Diffie Hellman Algorithm that exchanges keys securely by 
encrypting them. This is done by using the formula of Modular Exponentation.
The steps of this algorithm are described below.
=================================================

- Alice and Bob agree to use a prime number p and base g. (These two
values are chosen in this way to ensure that the resulting shared secret
can take on any value from 1 to p–1)
- Alice chooses a secret integer a<p, then sends Bob: A = g
a mod p.
- Bob chooses a secret integer b<p, then sends Alice: B = g
b mod p.
- Alice computes s = Ba mod p.
- Bob computes s = A
b mod p.
- Alice and Bob now share a secret

The tool will receive the required arguments from the command line upon execution as
such:
Options:
-o path Path to output file
-p number Prime number
-g number Primitive Root for previous prime number
-a number Private key A
-b number Private key B
-h This help message
The argument -p will include the will be the public prime number.
The argument -g will be the public primitive root of the previous prime number.
The argument -a will be the private key of user A.
The argument -b will be the private key of user B.

======  RSA Tool    ======
The RSA Algorithm involves two keys, i.e. a public key and a private
key. One key can be used for encrypting a message which can only be decrypted by the
other key. As an example let’s say we have two peers communicating with each other in
a channel secured by the RSA algorithm. The sender will encrypt the plain text with the
recipient’s public key. Then the receiver is the only one who can decrypt the message
using their private key. The public key will be available in a public key repository. n the
recipient’s side.
===========================

The tool will receive the required arguments from the command line upon execution as
such:
Options:
-i path Path to the input file
-o path Path to the output file
-k path Path to the key file
-g Perform RSA key-pair generation
-d Decrypt input and store results to output
-e Encrypt input and store results to output
-h This help message
The arguments “i”, “o” and “k” are always required when using “e” or “d”
Using -i and a path the user specifies the path to the input file.
Using -o and a path the user specifies the path to the output file.
Using -k and a path the user specifies the path to the key file.
Using -g the tool generates a public and a private key and stores them to the public.key
and private.key files respectively.
Using -d the user specifies that the tool should read the ciphertext from the input file,
decrypt it and then store the plaintext in the output file.



Compiler used for building this project:
gcc (Ubuntu 11.3.0-1ubuntu1~22.04) 11.3.0

