.section ".text.boot"

.global start
start:

    #Write the temporary pagetable addresses
    ldr r4, =temp_pagetable - 0xC0000000
    mcr p15, 0, r4, c2, c0, 0
    ldr r4, =temp_pagetable - 0xC0000000
    mcr p15, 0, r4, c2, c0, 1

    #Set the boundary to 0, all 4GB is paged by TTBR0
    ldr r4, =0
    mcr p15, 0, r4, c2, c0, 2
        
    #Set domain permissions
    ldr r4, =0x55555555
    mcr p15, 0, r4, c3, c0, 0

    #Enable paging
    ldr r5, =0xCF7FCBF8
    ldr r6, =0x00801807
    mrc p15, 0, r4, c1, c0, 0
    and r4, r4, r5
    orr r4, r4, r6
    mcr p15, 0, r4, c1, c0, 0

    #We're now paged using virtual memory.
    #Lets move to higher memory
    ldr pc, =high_addr

high_addr:

    #Now we're in higher memory lets unmap lower memory
    ldr r4, =kernel_pagetable - 0xC0000000
    mcr p15, 0, r4, c2, c0, 0
    ldr r4, =kernel_pagetable - 0xC0000000
    mcr p15, 0, r4, c2, c0, 1

    #Clear the TLB cache
    ldr r4, =0x00000000
    mcr p15, 0, r4, c8, c7, 0

    ldr sp, =0xC0008000

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
.global temp_pagetable

temp_pagetable:
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
    .fill 16, 4, 0x20040406

    #Unmapped (0x21000000-0xBFFFFFFF)
    .fill 4*2544, 1, 0

    #Linear map first 256MB of memory (0xC0000000-0xCFFFFFFF)
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

    #Linear map second 256MB of memory (0xD0000000-0xDFFFFFFF)
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

    #Unmapped (0xE0000000-0xF1FFFFFF)
    .fill   4*288, 1, 0

    #Linear map peripherals (0xF2000000-0xF2FFFFFF)
    #Using same layout as linux, as given by
    #BCM2825-ARM-Peripherals manual
    #Memory = device
    .fill	16, 4, 0x20040406

    #Unmapped (0xF3000000-0xFFFFFFFF)
    .fill   4*208, 1, 0

.balign 16384
.global kernel_pagetable

kernel_pagetable:
    #Unmapped (0x00000000-0xBFFFFFFF)
    .fill 4*3072, 1, 0

    #Linear map first 256MB of memory (0xC0000000-0xCFFFFFFF)
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

    #Linear map second 256MB of memory (0xD0000000-0xDFFFFFFF)
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

    #Unmapped (0xE0000000-0xF1FFFFFF)
    .fill   4*288, 1, 0
        
    #Linear map peripherals (0xF2000000-0xF2FFFFFF)
    #Using same layout as linux, as given by
    #BCM2825-ARM-Peripherals manual
    #Memory = device
    .fill	16, 4, 0x20040406

    #Unmapped (0xF3000000-0xFFFFFFFF)
    .fill   4*208, 1, 0
