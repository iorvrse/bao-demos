#include <config.h>

VM_IMAGE(linux_image, XSTR(BAO_DEMOS_WRKDIR_IMGS/linux.bin))

struct config config = {
    
    CONFIG_HEADER

    .shmemlist_size = 1,
    .shmemlist = (struct shmem[]) {
        [0] = { .size = 0x00010000, }
    },
    
    .vmlist_size = 1,
    .vmlist = (struct vm_config[]) {
        {
            .image = {
                .base_addr = 0x80200000,
                .load_addr = VM_IMAGE_OFFSET(linux_image),
                .size = VM_IMAGE_SIZE(linux_image)
            },

            .entry = 0x80200000,
            .cpu_affinity = 0x3, // 0b000011 - CPU0 & CPU1

            .platform = {
                .cpu_num = 2,
                
                .region_num = 1,
                .regions =  (struct vm_mem_region[]) {
                    {
                        .base = 0x80200000,
                        .size = 0x40000000, // 1 GB
                        .place_phys = true, // identity mapping
                        .phys = 0x80200000,
                    }
                },

                .ipc_num = 1,
                .ipcs = (struct ipc[]) {
                    {
                        .base = 0xf0000000,
                        .size = 0x00010000,
                        .shmem_id = 0,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {52}
                    }
                },

                .dev_num = 14,
                .devs =  (struct vm_dev_region[]) {   
                    {   
                        /* Arch timer interrupt */
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {27}
                    },
                    {
                        /* lpuart1 */
                        .pa = 0x5a070000,
                        .va = 0x5a070000,
                        .size = 0x10000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {378}
                    },
                    {
                        /* lpuart1 clock generator */
                        .pa = 0x5a470000,
                        .va = 0x5a470000,
                        .size = 0x10000,
                    },
                    {   
                        /* fec1 - ethernet */
                        .pa = 0x5b040000,
                        .va = 0x5b040000,
                        .size = 0x10000,
                        .interrupt_num = 4,
                        .interrupts = (irqid_t[]) {288, 289, 290, 291},    
                        .id = 0x2,
                    },
                    {   
                        /* enet0 clock generator */
                        .pa = 0x5b230000,
                        .va = 0x5b230000,
                        .size = 0x10000,
                    },
                    {
                        /* lsio_gpio1 (Ethernet PHY Reset & Interrupt) */
                        .pa = 0x5d090000,
                        .va = 0x5d090000,
                        .size = 0x10000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {169}
                    },
                    {
                        /* usdhc1 - eMMC */
                        .pa = 0x5b010000,
                        .va = 0x5b010000,
                        .size = 0x10000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {264},
                        .id = 0x3
                    },
                    {
                        /* usdhc1 clock generator */
                        .pa = 0x5b200000,
                        .va = 0x5b200000,
                        .size = 0x10000,
                    },
                    {
                        /* lsio_mu1 message unit */
                        .pa = 0x5d1c0000,
                        .va = 0x5d1c0000,
                        .size = 0x10000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {209},
                    },
                    {
                        /* lsio_mu5 message unit */
                        .pa = 0x5d200000,
                        .va = 0x5d200000,
                        .size = 0x10000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {216},
                    },
                    {
                        /* lsio_mu6 message unit */
                        .pa = 0x5d210000,
                        .va = 0x5d210000,
                        .size = 0x10000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {217},
                    },
                    {
                        /* usb3_lpcg */
                        .pa = 0x5b280000,
                        .va = 0x5b280000,
                        .size = 0x10000,
                    },
                    {
                        /* usbotg3 */
                        .pa = 0x5b110000,
                        .va = 0x5b110000,
                        .size = 0x50000,
                        .interrupt_num = 1,
                        .interrupts = (irqid_t[]) {303}
                    },
                    {
                        /* usb3_phy */
                        .pa = 0x5b160000,
                        .va = 0x5b160000,
                        .size = 0x40000,
                    },
                },
                
                .arch = {
                    .gic = {
                        .gicd_addr = 0x51a00000,
                        .gicr_addr = 0x51b00000
                    },
                }
            },
        },
    },
};