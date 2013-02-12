.section ".text.boot"

.global start
start:

    mov sp,#0x8000

    ldr r4,=_bss_start
    ldr r9,=_bss_end
    mov r5,#0
    mov r6,#0
    mov r7,#0
    mov r8,#0

1:
    stmia r4!, {r5-r8}

    cmp r4, r9
    blo 1b

    ldr r3, =kernel_main
    blx r3

halt:
    wfe
    b halt

