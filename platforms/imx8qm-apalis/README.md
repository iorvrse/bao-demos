# Toradex Apalis IMX8QM

## 1) Setup firmware

Create a directory to put the imx build tools:

```shell
export BAO_DEMOS_NXP_TOOLS=$BAO_DEMOS_WRKDIR_SRC/nxp-tools
cd $BAO_DEMOS_NXP_TOOLS
```

### 1.1) SCFW

Sign in and download the nxp SCFW porting kit [L6.6.52_2.2.2_SCFWKIT-1.17.0](https://www.nxp.com/webapp/sps/download/license.jsp?colCode=L6.6.52_2.2.2_SCFWKIT-1.17.0&appType=null&DOWNLOAD_ID=null)
Put the file in imx build tools directory and extract it

```shell
tar xvf imx-scfw-porting-kit-1.17.0.tar.gz
cd imx-scfw-porting-kit-1.17.0/packages/imx-scfw-porting-kit-1.17.0/src/scfw_export_mx8qm_b0.tar.gz
tar xvf scfw_export_mx8qm_b0.tar.gz
```

Clone the toradex SCFW, copy them to scfw porting kit directory and build

```shell
git clone https://github.com/toradex/i.MX-System-Controller-Firmware.git\
    $BAO_DEMOS_NXP_TOOLS --depth=1
cp -r $BAO_DEMOS_NXP_TOOLS/i.MX-System-Controller-Firmware/src/scfw_export_mx8qm_b0/* \
    imx-scfw-porting-kit-1.17.0/packages/imx-scfw-porting-kit-1.17.0/src/scfw_export_mx8qm_b0/
cd $BAO_DEMOS_NXP_TOOLS/imx-scfw-porting-kit-1.17.0/packages/imx-scfw-porting-kit-1.17.0/src/scfw_export_mx8qm_b0/
make SOC=MX8QM B=apalis DL=2 R=b0 U=0 V=0 qm
```

### 1.2) SECO

Download and install the seco binary:

```shell
wget -P $BAO_DEMOS_NXP_TOOLS\
    https://www.nxp.com/lgfiles/NMG/MAD/YOCTO/imx-seco-5.9.0.bin
cd $BAO_DEMOS_NXP_TOOLS
chmod a+x imx-seco-5.9.0.bin
./imx-seco-5.9.0.bin
```

## 1.3) Build U-boot

Setup the u-boot directory variable:

```shell
export BAO_DEMOS_UBOOT=$BAO_DEMOS_WRKDIR_SRC/u-boot
```

Download, configure it:

```shell
git clone https://git.toradex.com/u-boot-toradex.git $BAO_DEMOS_UBOOT\
    --depth 1 --branch toradex_imx_lf_v2022.04
cd $BAO_DEMOS_UBOOT
make apalis-imx8_defconfig
make -j $(nproc)
```

And copy the image to the platform's working directory:

```shell
cp $BAO_DEMOS_UBOOT/u-boot.bin $BAO_DEMOS_WRKDIR_PLAT
```

### 1.4) Build TF-A

Setup the TF-A directory variable:

```shell
export BAO_DEMOS_ATF=$BAO_DEMOS_WRKDIR_SRC/arm-trusted-firmware
```

Download and build it:

```shell
git clone https://github.com/nxp-imx/imx-atf.git\
    $BAO_DEMOS_ATF --branch lf_v2.6 --depth 1
cd $BAO_DEMOS_ATF
git apply $BAO_DEMOS/platforms/imx8qm-apalis/0001-Makefile-Suppress-array-bounds-error.patch
git apply $BAO_DEMOS/platforms/imx8qm-apalis/0001-Revert-Add-NXP-s-SoCs-partition-reboot-support.patch
git apply $BAO_DEMOS/platforms/imx8qm-apalis/0002-imx8m-hab.c-work-around-gcc-12.1-false-positives.patch
git apply $BAO_DEMOS/platforms/imx8qm-apalis/0001-add-imx8-smmu-stream-id-initialization.patch
make PLAT=imx8qm bl31
```

And copy the image to the platform's working directory:

```shell
cp $BAO_DEMOS_ATF/build/imx8qm/release/bl31.bin $BAO_DEMOS_WRKDIR_PLAT
```


### 1.5) NXP's mkimage tool

```shell
git clone https://github.com/nxp-imx/imx-mkimage.git\
    $BAO_DEMOS_NXP_TOOLS/imx-mkimage --depth 1 --branch lf-5.15.71-2.2.0
git apply $BAO_DEMOS/platforms/imx8qm-apalis/0001-iMX8M-soc.mak-use-native-mkimage-from-sysroot.patch
```

Go to imx-mkimage target platform directory and copy over the scfw seco firmware
as well as the TF-A's bl31 and uboot binaries. Then build firmware binary.

```shell
cd $BAO_DEMOS_NXP_TOOLS/imx-mkimage
cp $BAO_DEMOS_NXP_TOOLS/imx-scfw-porting-kit-1.17.0/packages/imx-scfw-porting-kit-1.17.0/src/scfw_export_mx8qm_b0/build_mx8qm_b0/scfw_tcm.bin\
    iMX8QM
cp $BAO_DEMOS_NXP_TOOLS/imx-seco-5.9.0/firmware/seco/mx8qmb0-ahab-container.img\
    iMX8QM
cp $BAO_DEMOS_WRKDIR_PLAT/bl31.bin iMX8QM
cp $BAO_DEMOS_WRKDIR_PLAT/u-boot.bin iMX8QM
make SOC=iMX8QM flash_b0
cp iMX8QM/flash.bin $BAO_DEMOS_WRKDIR_IMGS
```

Or in case you wanna run m4 cores too, copy your m4 binaries to the imx-mkimage target platform directory and build with command below

```shell
cp <path-to-m4_0-binary> iMX8QM/m4_image.bin
cp <path-to-m4_1-binary> iMX8QM/m4_1_image.bin
make SOC=iMX8QM flash_regression_linux_m4
cp iMX8QM/flash.bin $BAO_DEMOS_WRKDIR_IMGS
```

<!--- instruction#1 -->
## 2) Setup Tezi image folder

Finally, [prepare your tezi image](./../../platforms/tezi.md), and copy bao's final image to it:

<!--- instruction#2 -->
## 3) Setup board

Connect to the  UART via the USB cable. If you are using the dual guest 
configuration, connect to both available UARTs (in our setup /dev/ttyUSB0)
with baud 115200.

Open a new terminal and connect to it. For example:

```
screen /dev/ttyUSB0 115200
```
 
Reset the board.

You should able to see your OS boot logs.