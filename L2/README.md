# Interruptions

This Folder contains 2 interruption exercises. The first one implements the int 40h interruption to print the string "AP2 de IHS" with some requirements. The second, takes the exercise in the L1 folder and implements 3 functions ("printInterface", "cadastrar" and "printBuscar") as an ISR located in position 20H of the IVT.

To compile and execute the 1st exercise, run the following lines of code on shell:

    nasm exerc1.asm -o exerc1.bin
    qemu-system-i386 exerc1.bin

In order to execute the 2nd exercise, run the following code:

    make all

