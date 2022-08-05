HOMEPAGE = "http://www.denx.de/wiki/U-Boot/WebHome"
SECTION = "bootloaders"

LICENSE = "GPLv2+"
LIC_FILES_CHKSUM = "file://Licenses/README;md5=a2c678cfd4a4d97135585cad908541c6"
PE = "1"

require recipes-bsp/u-boot/u-boot.inc

DEPENDS += "bc-native dtc-native"

UBOOTURI ?= "git://github.com/Digilent/u-boot-digilent.git;protocol=https"
UBRANCH ?= "master"
#UBRANCHARG = "${@['nobranch=1', 'branch=${UBRANCH}'][d.getVar('UBRANCH', True) != '']}"
SRC_URI = "${UBOOTURI};nobranch=1"
SRCREV = "digilent-v${PV}"

S = "${WORKDIR}/git"

FILESEXTRAPATHS_prepend := "${THISDIR}/u-boot:"

COMPATIBLE_MACHINE_append = "pynq"

SRC_URI_append = "\
	file://0001-Embed-device-tree-for-ArtyZ7-board.patch;striplevel=1 \
	file://0001-usb_storage-Change-the-USB_MAX_XFER_BLK-value.patch;striplevel=1 \
	"

