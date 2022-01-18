.macro blh to, reg=r3
    ldr \reg, =\to
    mov lr, \reg
    .short 0xF800
.endm

.thumb
push {lr}

ldr r1, =#0x30004B8
ldr r0, [r1, #0x4]
ldr r1, =#0x202BD09
strb r0, [r1]

pop {r3}
bx r3
.align
.ltorg
