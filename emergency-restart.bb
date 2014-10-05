DESCRIPTION = "Tool to restart E2 or box when box is not responding"
LICENSE = "GPLv2"
LIC_FILES_CHKSUM = "file://LICENSE;md5=8264535c0c4e9c6c335635c4026a8022"

S = "${FILE_DIRNAME}/"

WORKDIR = "${S}/build/"

PR = "r0"

EXTRA_OEMAKE = "'CPPFLAGS=${CXXFLAGS}'"