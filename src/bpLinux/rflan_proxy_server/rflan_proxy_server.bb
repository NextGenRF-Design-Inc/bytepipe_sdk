SUMMARY = "RFLAN CLI Proxy Server"

LICENSE = "BSD"
LIC_FILES_CHKSUM = "file://LICENSE;md5=b30cbe0b980e98bfd9759b1e6ba3d107"

SRC_URI = "\
	file://LICENSE \
	file://Makefile \
	file://rflan_proxy_server.c \
	file://rflan_proxy_server.h \
	"

S = "${WORKDIR}"

RRECOMMENDS_${PN} = "kernel-module-rpmsg-char"

FILES_${PN} = "\
	/usr/bin/rflan_proxy_server \
"

do_install () {
	install -d ${D}/usr/bin
	install -m 0755 rflan_proxy_server ${D}/usr/bin/rflan_proxy_server
}
