@echo off 
rem cls

@echo [HIMAX] ************************************************  
@echo [HIMAX] ************** Menuconfig Script ***************  
@echo [HIMAX] ************************************************  
@echo [HIMAX]
 
rem ### chip_name start ###
SETLOCAL EnableDelayedExpansion
set chip_name_path=ini_folder\chip_name.ini
@echo [HIMAX] ---------------- Chip Name List ---------------- 

:chip_name_list
set /a chip_name_cnt=1
set /a chip_tmp_cnt=1
Set /a aa=0
for /f "delims=" %%i in (%chip_name_path%) do (
    echo [HIMAX] !chip_name_cnt!. %%i
	set /a chip_name_cnt += 1
)
set /a chip_tmp_cnt = chip_name_cnt - 2

@echo [HIMAX]
set /p chip_name_input="[HIMAX] Please select one option ( 1 ~ %chip_tmp_cnt% ): "

if %chip_name_input% leq %chip_tmp_cnt% (
	if %chip_name_input% gtr 0 (
		For /f "delims=" %%c in (%chip_name_path%) do (
			Set /a aa += 1
			rem echo [cdc] !aa!. %%c
			if !aa! equ %chip_name_input% set tmp_chip_name=%%c
		)
	) else (
		@echo [HIMAX]
		@echo [HIMAX] Your selection %chip_name_input% is not on the list please check and try again!
		goto chip_name_list
	)
) else (
	@echo [HIMAX]
	@echo [HIMAX] Your selection %chip_name_input% is not on the list please check and try again!
	goto chip_name_list
)

@echo [HIMAX] Your selection is "%tmp_chip_name%"
rem ### chip_name end ###


@echo [HIMAX]


rem ### config_mode start ###
SETLOCAL EnableDelayedExpansion
set config_mode_path=ini_folder\config_mode.ini
@echo [HIMAX] ---------------- Config Mode List  ---------------- 

:config_mode_list
set /a config_mode=1
set /a config_mode_cnt=1
Set /a dd=0
for /f "delims=" %%i in (%config_mode_path%) do (
    echo [HIMAX] !config_mode!. %%i
	set /a config_mode += 1
)
set /a config_mode_cnt = config_mode - 2

@echo [HIMAX]
set /p config_mode_input="[HIMAX] Please select one option ( 1 ~ %config_mode_cnt% ): "

if %config_mode_input% leq %config_mode_cnt% (
	if %config_mode_input% gtr 0 (
		For /f "delims=" %%c in (%config_mode_path%) do (
			Set /a dd += 1
			if !dd! equ %config_mode_input% set config_mode_name=%%c
		)
	) else (
		@echo [HIMAX]
		@echo [HIMAX] Your selection %config_mode_input% is not on the list please check and try again!
		goto config_mode_list
	)
) else (
	@echo [HIMAX]
	@echo [HIMAX] Your selection %config_mode_input% is not on the list please check and try again!
	goto config_mode_list
)

@echo [HIMAX] Your selection is "%config_mode_name%"
rem ### config_mode end ###


@echo [HIMAX]


rem ### sec_format start ###
SETLOCAL EnableDelayedExpansion
set sec_format_path=ini_folder\sec_format.ini
@echo [HIMAX] ---------------- Sec Format List ---------------- 

:sec_format_list
set /a sec_format=1
set /a sec_format_cnt=1
Set /a bb=0
for /f "delims=" %%i in (%sec_format_path%) do (
    echo [HIMAX] !sec_format!. %%i
	set /a sec_format += 1
)
set /a sec_format_cnt = sec_format - 2

@echo [HIMAX]
set /p sec_format_input="[HIMAX] Please select one option ( 1 ~ %sec_format_cnt% ): "

if %sec_format_input% leq %sec_format_cnt% (
	if %sec_format_input% gtr 0 (
		For /f "delims=" %%c in (%sec_format_path%) do (
			Set /a bb += 1
			if !bb! equ %sec_format_input% set sec_format_name=%%c
		)
	) else (
		@echo [HIMAX]
		@echo [HIMAX] Your selection %sec_format_input% is not on the list please check and try again!
		goto sec_format_list
	)
) else (
	@echo [HIMAX]
	@echo [HIMAX] Your selection %sec_format_input% is not on the list please check and try again!
	goto sec_format_list
)

@echo [HIMAX] Your selection is "%sec_format_name%"
if %sec_format_input% equ 1 (set ini_sign_forma=sign_formal) else if %sec_format_input% equ 2 (set ini_sign_forma=enc_formal2) else (set ini_sign_forma=sign_formal)
rem ### sec_format end ###


@echo [HIMAX]


rem ### flash_max_size start ###
SETLOCAL EnableDelayedExpansion
set flash_max_size_path=ini_folder\flash_max_size.ini
@echo [HIMAX] ---------------- Max Flash Size List ---------------- 

:flash_max_size_list
set /a flash_max_size=1
set /a flash_max_size_cnt=1
Set /a cc=0
for /f "delims=" %%i in (%flash_max_size_path%) do (
    echo [HIMAX] !flash_max_size!. %%i
	set /a flash_max_size += 1
)
set /a flash_max_size_cnt = flash_max_size - 2

@echo [HIMAX]
set /p flash_max_size_input="[HIMAX] Please select one option ( 1 ~ %flash_max_size_cnt% ): "

if %flash_max_size_input% leq %flash_max_size_cnt% (
	if %flash_max_size_input% gtr 0 (
		For /f "delims=" %%c in (%flash_max_size_path%) do (
			Set /a cc += 1
			if !cc! equ %flash_max_size_input% set flash_max_size_name=%%c
		)
	) else (
		@echo [HIMAX]
		@echo [HIMAX] Your selection %flash_max_size_input% is not on the list please check and try again!
		goto flash_max_size_list
	)
) else (
	@echo [HIMAX]
	@echo [HIMAX] Your selection %flash_max_size_input% is not on the list please check and try again!
	goto flash_max_size_list
)

@echo [HIMAX] Your selection is "%flash_max_size_name%"
if %flash_max_size_input% equ 1 (set ini_flash_size=0x100000) else if %flash_max_size_input% equ 2 (set ini_flash_size=0x200000) else (set ini_flash_size=0x100000)
rem ### flash_max_size end ###


@echo [HIMAX]


rem ### part_number start ###
if %chip_name_input% equ 1 set chip_pin_name=part_number_38.ini
if %chip_name_input% equ 2 set chip_pin_name=part_number_72.ini
if %chip_name_input% equ 3 set chip_pin_name=part_number_128.ini

SETLOCAL EnableDelayedExpansion
set part_number_path=ini_folder\part_number\%chip_pin_name%
@echo [HIMAX] ---------------- %tmp_chip_name% Part Number List ---------------- 

:part_number_list
set /a part_number=1
set /a part_number_cnt=1
Set /a ee=0
for /f "delims=" %%i in (%part_number_path%) do (
    echo [HIMAX] !part_number!. %%i
	set /a part_number += 1
)
set /a part_number_cnt = part_number - 2

@echo [HIMAX]
set /p part_number_input="[HIMAX] Please select one option ( 1 ~ %part_number_cnt% ): "

if %part_number_input% leq %part_number_cnt% (
	if %part_number_input% gtr 0 (
		For /f "delims=" %%c in (%part_number_path%) do (
			Set /a ee += 1
			if !ee! equ %part_number_input% set part_number_name=%%c
		)
	) else (
		@echo [HIMAX]
		@echo [HIMAX] Your selection %part_number_input% is not on the list please check and try again!
		goto part_number_list
	)
) else (
	@echo [HIMAX]
	@echo [HIMAX] Your selection %part_number_input% is not on the list please check and try again!
	goto part_number_list
)

@echo [HIMAX] Your selection is "%part_number_name%"
rem ### part_number end ###


echo [HIMAX]
echo [HIMAX]


rem ### Generate image_gen_config.ini start ###
(
	echo [PROJECT_BASE]
	echo flash_max_size = %ini_flash_size%
	echo oupt_file = output\output.img
	echo odm_wrapkey = odm_key\%tmp_chip_name%_%sec_format_name%_%config_mode_name%\%part_number_name%\odm_wrapkey.key
	echo pubkey = odm_key\%tmp_chip_name%_%sec_format_name%_%config_mode_name%\%part_number_name%\we1_root_rsa_key.der.pub
	echo prikey = odm_key\%tmp_chip_name%_%sec_format_name%_%config_mode_name%\%part_number_name%\odm_rsa_key.der
	echo cert = odm_key\%tmp_chip_name%_%sec_format_name%_%config_mode_name%\%part_number_name%\cert1_rsa.bin
	echo devision_size_limit = FB00  
	echo chip_name = %tmp_chip_name%
	echo config_mode = %config_mode_name%
	echo part_number = %part_number_name%
	echo:
	echo [BOOTLOADER]
	echo pat_type = 0
	echo input_file = input\%sec_format_name%\%tmp_chip_name%_%sec_format_name%_%config_mode_name%\%sec_format_name%_%part_number_name%\%ini_sign_forma%_PA8530_EM9D_Bootloader.bin
	echo sec_format = %sec_format_name%
	echo version = 2
	echo fw_type = 1
	echo flash_addr = 10000
	echo:
	echo [2ND_BOOTLOADER]
	echo pat_type = 1
	echo input_file = input\%sec_format_name%\%tmp_chip_name%_%sec_format_name%_%config_mode_name%\%sec_format_name%_%part_number_name%\%ini_sign_forma%_PA8530_EM9D_2nd_Bootloader.bin
	echo sec_format = %sec_format_name%
	echo version = 2
	echo fw_type = 1
	echo flash_addr = 0
	echo:
	echo [MEMORY_DESCIRPTOR]
	echo pat_type = 3
	echo output_file = output\layout
	echo sec_format = BLp
	echo version = 2
	echo fw_type = 4
	echo flash_addr = 20000
	echo:
	echo [APPLICATION]
	echo pat_type = 4
	echo input_file = input\WEI_FW_mw_arcem9d_wei_r16
	echo sec_format = %sec_format_name%
	echo version = 2
	echo fw_type = 3
	echo flash_addr = 21000
	echo devision_size = FB00
) > image_gen_config.ini

echo [HIMAX] *********************************************** 
echo [HIMAX] ******************* Success ******************* 
echo [HIMAX] *********************************************** 
echo [HIMAX] ***                                         *** 
echo [HIMAX] ***     image_gen_config.ini generated      *** 
echo [HIMAX] ***                                         *** 
echo [HIMAX] *********************************************** 
echo [HIMAX] ******************* Success ******************* 
echo [HIMAX] *********************************************** 
echo [HIMAX]
rem ### Generate image_gen_config.ini end ###