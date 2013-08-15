#!/usr/bin/python

file=open('AES')

	# Lines are KEY, PLAINTEXT, CIPHERTEXT
key=file.readline().split('=')[1]
plain=file.readline().split('=')[1]
cipher=file.readline().split('=')[1]
#if not cipher: break
print key



