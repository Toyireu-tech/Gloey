Just my personnal CPU emulator

Calculate the 10th fib number
jump _start

.section fib
    load r7 1

    .label fib_loop

        add r11 r6 r7
        copy r6 r7
        copy r7 r11

        addi r10 r10 1
        comp r10 r1
        jneq fib_loop
        
    copy r0 r6
    ret

.label _start
    load r1 10
    call fib
halt

