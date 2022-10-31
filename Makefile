default: dhel rsa
	
dhel:
	gcc dh_assign_1.c -o dh_assign_1 -lgmp

rsa:
	gcc rsa_assign_1.c -o rsa_assign_1 -lgmp

clean:
	rm -f dh_assign_1 rsa_assign_1 public.key private.key ciphertext.txt
