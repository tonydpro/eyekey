# eyekey
// This file explains the purpose of Eyekey and its functionning

Part I : Basic description of the program

Eyekey is a software for cryptography using two key to encrypt files.

-> The first key is a classic password

-> The second key is a QRcode like pictogram, which I call "visual key" in this document

The visual key is a QRcode composed of an array of 10 lines and 10 columns.

The right and left tops and bottoms of the visual key are used by the program to identify the limits of the visual key, and so the array contains 100 - 4 = 96 cases.

Every case can be either black or white, which allows 2^96 possibles combinations.

If you add the fact that the visual key doesn't give any indication on the way it has to be used, their is in fact 4 time more possible combinations.

Which makes in the end, 2^100 possibles values for the visual key which is a plus for security.

The program provides an easy way to generate a random visual key bitmap that can be printed or stored in a smartphone.

For a better security, I recommand to print the key instead of storing it in any device. By the way, the bitmap is automaticallay deleted in a secure way after the end of the program execution to make the process of key generation more secure.


