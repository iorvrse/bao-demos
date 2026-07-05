## Generating Tezi Compatible Image

To flash an image via Toradex Easy Installer, it is necessary to generate a Tezi-compatible image. The links below offer some guidance on how to generate, customize and build an image using Yocto Project.

Regarding the usage of Yocto Project to build an image, you can read the following articles:
- [Build a Reference Image with Yocto Project/OpenEmbedded](https://developer.toradex.com/linux-bsp/os-development/build-yocto/build-a-reference-image-with-yocto-projectopenembedded)
- [Reference Images for Yocto Project Software Downloads](https://developer.toradex.com/software/toradex-embedded-software/toradex-download-links-torizon-linux-bsp-wince-and-partner-demos#toradex-embedded-linux---yocto-project-reference-images)

### i) Make new bootfs

From the Tezi-compatible image we will replace the default bootfs with new bootfs contained bao binary and boot script to load the bao. The boot script only do a command to load bao at 0xE5000000 from eMMC partition 1.

```shell
cd <path-to-tezi-image>
mkdir bootfs
cd bootfs

echo "echo "Booting Bao Hypervisor..."
setenv loadaddr 0xE5000000
fatload mmc ${devnum}:1 ${loadaddr} bao.bin
go ${loadaddr}" > boot.cmd

mkimage -A arm -O linux -T script -C none -a 0 -e 0 -n "Bao Boot Script" -d boot.cmd boot.scr

cp $BAO_DEMOS_WRKDIR_IMGS/bao.bin .
tar -cJf bao-bootfs.tar.xz bao.bin boot.scr
mv bao-bootfs.tar.xz ./..
```

The directory would look something like this

```shell
.
├── bao.bootfs.tar.xz
├── bao.png
├── bootfs
│   ├── bao.bin
│   ├── boot.cmd
│   └── boot.scr
├── image.json
├── imx-boot
├── LA_OPT_NXP_SW.html
├── marketing.tar
├── prepare.sh
├── RT-Minimal-Image-Bao-rt-apalis-imx8.tar.xz  # this is my image example
├── u-boot-initial-env-sd
└── wrapup.sh
```

### ii) Update image.json

We need to update image.json to use the new bootfs. Here is example of the image.json

```json
{
    "config_format": "4",
    "autoinstall": false,
    "name": "Toradex Bao Image (Yocto PREEMPT_RT)",
    "description": "Minimal Linux PREEMPT-RT Image for Bao Hypervisor (PREEMPT_RT).",
    "version": "6.8.5-devel-20260512020010+build.0",
    "release_date": "2026-05-12",
    "u_boot_env": "u-boot-initial-env-sd",
    "prepare_script": "prepare.sh",
    "wrapup_script": "wrapup.sh",
    "marketing": "marketing.tar",
    "icon": "bao.png",
    "license": "LA_OPT_NXP_SW.html",
    "supported_product_ids": [
        "0037",
        "0047",
        "0048",
        "0049",
        "0067",
        "0091",
        "0092",
        "0093",
        "0094",
        "0095"
    ],
    "blockdevs": [
        {
            "name": "mmcblk0",
            "partitions": [
                {
                    "partition_size_nominal": 64,
                    "want_maximised": false,
                    "content": {
                        "label": "BOOT",
                        "filesystem_type": "FAT",
                        "mkfs_options": "",
                        // "filename": "RT-Minimal-Image-Bao-rt-apalis-imx8.bootfs.tar.xz",
                        "filename": "bao.bootfs.tar.xz",
                        "uncompressed_size": 55
                    }
                },
                {
                    "partition_size_nominal": "512",
                    "want_maximised": true,
                    "content": {
                        "label": "RFS",
                        "filesystem_type": "ext4",
                        "mkfs_options": "-E nodiscard",
                        "filename": "RT-Minimal-Image-Bao-rt-apalis-imx8.tar.xz",
                        "uncompressed_size": 200.98828125
                    }
                }
            ]
        },
        {
            "name": "mmcblk0boot0",
            "erase": true,
            "content": {
                "filesystem_type": "raw",
                "rawfiles": [
                    {
                        "filename": "imx-boot",
                        "dd_options": "seek=0"
                    }
                ]
            }
        }
    ]
}
```

### iii) Flash image

We can copy the tezi image folder to flashdisk and flash the image. For the information on how to flash OS image using Tezi, refer to [Flash a New Image Using Toradex Easy Installer](https://developer.toradex.com/easy-installer/toradex-easy-installer/flashing-new-image-using-tezi#flash-a-toradex-pre-built-image).

If you have already flashed an OS into your Computer-on-Module, you can follow the steps to reload Tezi into the RAM of the module, so you can flash it with the desired image.

- [Download a new Toradex Easy Installer image](https://developer.toradex.com/easy-installer/toradex-easy-installer/download-tezi).
- Enter in [Recovery Mode](https://developer.toradex.com/hardware/hardware-resources/recovery-mode/). Refer to the article that is suitable for the specific module you are using.
- Then [Load the Toradex Easy Installer](https://developer.toradex.com/easy-installer/toradex-easy-installer/loading-toradex-easy-installer) to the internal flash.
After these steps, you will be back at the main graphical user interface and be able to easily install an OS image.

## Reference
- [Toradex Developer Center](https://developer.toradex.com)