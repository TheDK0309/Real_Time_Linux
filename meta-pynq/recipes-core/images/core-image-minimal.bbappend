
# imported and base recipes
IMAGE_INSTALL += "\
    rt-tests \
    dropbear \
    "

# shell script to generate static key for dropbear
configure_dropbear() {
    if [ -x /usr/local/bin/dropbearkey ]; then
        if [ ! -f ${IMAGE_ROOTFS}/etc/dropbear/dropbear_rsa_host_key ]; then
             /usr/local/bin/dropbearkey -t rsa -f ${IMAGE_ROOTFS}/etc/dropbear/dropbear_rsa_host_key
        fi
    fi
}

ROOTFS_POSTPROCESS_COMMAND += "configure_dropbear;"
