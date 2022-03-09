    .text
    .balign 4
    .global mem
mem:
    li t0, 0x10000
    li t1, 0x11
    sd t1, (t0)
    ld t1, (t0)
    ret
