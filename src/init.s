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

.section ".data.pagetable"

	.balign 16384
	.global kernel_pagetable
kernel_pagetable:
	#Identity map first 256MB of memory (0x00000000-0x0FFFFFFF)
	# write-back, allocate on write
	.fill 16, 4, 0x0004140E
	.fill 16, 4, 0x0104140E
	.fill 16, 4, 0x0204140E
	.fill 16, 4, 0x0304140E
	.fill 16, 4, 0x0404140E
	.fill 16, 4, 0x0504140E
	.fill 16, 4, 0x0604140E
	.fill 16, 4, 0x0704140E
	.fill 16, 4, 0x0804140E
	.fill 16, 4, 0x0904140E
	.fill 16, 4, 0x0A04140E
	.fill 16, 4, 0x0B04140E
	.fill 16, 4, 0x0C04140E
	.fill 16, 4, 0x0D04140E
	.fill 16, 4, 0x0E04140E
	.fill 16, 4, 0x0F04140E

        #Identity map second 256MB of memory (0x10000000-0x1FFFFFFF)
        # write-through, no allocate on write
	.fill 16, 4, 0x1004040A
	.fill 16, 4, 0x1104040A
	.fill 16, 4, 0x1204040A
	.fill 16, 4, 0x1304040A
	.fill 16, 4, 0x1404040A
	.fill 16, 4, 0x1504040A
	.fill 16, 4, 0x1604040A
	.fill 16, 4, 0x1704040A
	.fill 16, 4, 0x1804040A
	.fill 16, 4, 0x1904040A
	.fill 16, 4, 0x1A04040A
	.fill 16, 4, 0x1B04040A
	.fill 16, 4, 0x1C04040A
	.fill 16, 4, 0x1D04040A
	.fill 16, 4, 0x1E04040A
	.fill 16, 4, 0x1F04040A
        
	#Identity map peripherals too (0x20000000-0x20FFFFFF)
	#Memory = device
	.fill	16, 4, 0x20040406

	#Rest of memory is unmapped (0x21000000-0xFFFFFFFF)
	.fill   4*3568, 1, 0
