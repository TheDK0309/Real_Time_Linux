SUMMARY = "U-Boot uEnv.txt SD boot environment generation for Zynq targets"
LICENSE = "MIT"
LIC_FILES_CHKSUM = "file://${COMMON_LICENSE_DIR}/MIT;md5=0835ade698e0bcf8506ecda2f7b4f302"

INHIBIT_DEFAULT_DEPS = "1"
PACKAGE_ARCH = "${MACHINE_ARCH}"

COMPATIBLE_MACHINE = "pynq"

FILESEXTRAPATHS_prepend := "${THISDIR}/files:"

SRC_URI = " file://uEnv.txt"

inherit deploy

FILES_${PN} += "/boot/uEnv.txt"

do_install() {
	install -Dm 0644 ${WORKDIR}/uEnv.txt ${D}/boot/uEnv.txt
}

do_deploy() {
	install -Dm 0644 ${WORKDIR}/uEnv.txt ${DEPLOYDIR}/uEnv.txt
}
addtask do_deploy after do_compile before do_build

