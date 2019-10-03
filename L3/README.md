# Switching Modes

This folder contains 1st and 2nd exercises of IHS's AP3. The challenge was to switch from real to protected mode and than switch back to real mode. In each mode, a "Hello World %d" message is printed for the user, where %d indicates the correspondenting mode (16 - real mode and 32 - protected mode).

To compile and execute the exercise, run the following lines of code on shell:

    nasm exerc1.asm -o exerc1.bin
    qemu-system-i386 exerc1.bin

