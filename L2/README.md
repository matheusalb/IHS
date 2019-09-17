# Interruptions

This folder contains 2 interruption exercises. The first one implements interrupt int 40h which prints the string **AP2 de IHS** following some requirements. The second one performs the L1 folder exercise, implementing 3 of the functions (**printInterface**, **cadastrar** and **printBuscar**) as an ISR located at position 20H of IVT.

To compile and execute the **1st exercise**, run the following lines of code on shell:

    nasm exerc1.asm -o exerc1.bin
    qemu-system-i386 exerc1.bin

In order to execute the **2nd exercise**, run the following code on the exerc2 folder:

    make all

