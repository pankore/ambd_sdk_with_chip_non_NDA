cmake_minimum_required(VERSION 3.6)

project(target)

set(dir_crypto "${sdk_root}/component/soc/realtek/amebad/fwlib/crypto")
set(dir_fwlib "${sdk_root}/component/soc/realtek/amebad/fwlib/ram_hp")
set(dir_fwlib_com "${sdk_root}/component/soc/realtek/amebad/fwlib/ram_common")
set(dir_fwlib_cfg "${sdk_root}/component/soc/realtek/amebad/fwlib/usrcfg")
set(dir_misc "${sdk_root}/component/soc/realtek/amebad/misc")

list(
    APPEND ${list}

    # crypto
    ${dir_crypto}/rtl8721dhp_sha.c
	${dir_crypto}/rtl8721dhp_md5.c
	${dir_crypto}/rtl8721dhp_chacha_poly1305.c
	${dir_crypto}/rtl8721dhp_aes_ram.c
	${dir_crypto}/rtl8721dhp_des_ram.c
	${dir_crypto}/rtl8721dhp_crypto_ram.c

    # fwlib
    ${dir_fwlib}/rtl8721dhp_startup.c
	${dir_fwlib}/rtl8721dhp_app_start.c
	${dir_fwlib}/rtl8721dhp_clk.c
	${dir_fwlib}/rtl8721dhp_cpft.c
	${dir_fwlib}/rtl8721dhp_ota_ram.c
	${dir_fwlib}/rtl8721dhp_system.c
	${dir_fwlib}/rtl8721dhp_simulation.c
    ${dir_fwlib}/rtl8721dhp_i2s.c
	${dir_fwlib}/rtl8721dhp_sdio.c
	${dir_fwlib}/rtl8721dhp_ssi.c
	${dir_fwlib}/rtl8721dhp_lcdc.c
	${dir_fwlib}/rtl8721dhp_usi_uart.c
	${dir_fwlib}/rtl8721dhp_usi_ssi.c
	${dir_fwlib}/rtl8721dhp_usi_i2c.c
	${dir_fwlib}/rtl8721dhp_audio.c
	${dir_fwlib}/rtl8721dhp_sdio_host.c
	${dir_fwlib}/rtl8721dhp_sd.c

    # fwlib_com
    ${dir_fwlib_com}/rtl8721d_i2c.c
	${dir_fwlib_com}/rtl8721d_rtc.c
	${dir_fwlib_com}/rtl8721d_uart.c
	${dir_fwlib_com}/rtl8721d_tim.c
	${dir_fwlib_com}/rtl8721d_adc.c
	${dir_fwlib_com}/rtl8721d_comparator.c
	${dir_fwlib_com}/rtl8721d_wdg.c
	${dir_fwlib_com}/rtl8721d_flash_ram.c
	${dir_fwlib_com}/rtl8721d_pll.c
	${dir_fwlib_com}/rtl8721d_ir.c
	${dir_fwlib_com}/rtl8721d_ipc_api.c
	${dir_fwlib_com}/rtl8721d_sgpio.c
	${dir_fwlib_com}/rtl8721d_keyscan.c
	${dir_fwlib_com}/rtl8721d_captouch.c
	${dir_fwlib_com}/rtl8721d_gdma_memcpy.c
	${dir_fwlib_com}/rtl8721d_bor.c
	${dir_fwlib_com}/rtl8721d_ram_libc.c

    # fwlib_cfg
    ${dir_fwlib_cfg}/rtl8721dlp_intfcfg.c
    ${dir_fwlib_cfg}/rtl8721dhp_intfcfg.c
    ${dir_fwlib_cfg}/rtl8721d_ipccfg.c
    ${dir_fwlib_cfg}/rtl8721d_wificfg.c

    # misc
    ${dir_misc}/rtl8721d_ota.c

    # usb_otg
    # Unable to find file
)

list(
    APPEND ${list_inc_path}
    ${dir_crypto}/../include
)
