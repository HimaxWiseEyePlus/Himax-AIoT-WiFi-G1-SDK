/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */
#ifndef _TARGET_MEM_CONFIG_H_
#define _TARGET_MEM_CONFIG_H_

#include "arc_feature_config.h"

//#define BOOT_SPI_FLASH_SIZE		0x200000
//#define BOOT_SPI_FLASH_BASE		0x10000000

//#define ONCHIP_FLASH_SIZE		0x40000
//#define ONCHIP_FLASH_BASE		0x0


#ifdef USE_APPL_MEM_CONFIG
#include "appl_mem_config.h"
#endif

/**
 * The unit of XXXX_SIZE is Byte
 * For REGION_ROM, ICCM, EXT_ROM and EXT_RAM are available
 * For REGION_RAM, DCCM and EXT_RAM are available
 */
#ifdef ARC_FEATURE_ICCM_PRESENT

#ifdef ARC_FEATURE_ICCM0_PRESENT
#ifndef ICCM0_SIZE
#define ICCM0_SIZE	ARC_FEATURE_ICCM0_SIZE
#endif
#ifndef ICCM0_START
#define ICCM0_START	ARC_FEATURE_ICCM0_BASE
#endif
#endif	//ARC_FEATURE_ICCM0_PRESENT

#ifdef ARC_FEATURE_ICCM1_PRESENT
#ifndef ICCM1_SIZE
#define ICCM1_SIZE	0x50000
#endif
#ifndef ICCM1_START
#define ICCM1_START	ARC_FEATURE_ICCM1_BASE
#endif
#endif	//ARC_FEATURE_ICCM0_PRESENT

#if !defined(ARC_FEATURE_ICCM0_PRESENT) && !defined(ARC_FEATURE_ICCM1_PRESENT)
#ifndef ICCM_SIZE
#define ICCM_SIZE	ARC_FEATURE_ICCM_SIZE
#endif
#ifndef ICCM_START
#define ICCM_START	ARC_FEATURE_ICCM_BASE
#endif
#endif

#else
#ifndef ICCM_SIZE
#define ICCM_SIZE	0x40000
#endif
#ifndef ICCM_START
#define ICCM_START	0x10000000
#endif
#endif	//ARC_FEATURE_ICCM_PRESENT

#ifdef ARC_FEATURE_DCCM_PRESENT
#ifndef DCCM_SIZE
#define DCCM_SIZE	0x40000
#endif
#ifndef DCCM_START
#define DCCM_START	ARC_FEATURE_DCCM_BASE
#endif
#else
#ifndef DCCM_SIZE
#define DCCM_SIZE	0x20000
#endif
#ifndef DCCM_START
#define DCCM_START	0x80000000
#endif
#endif

#ifndef EXT_RAM_START
#define EXT_RAM_START	0x20000000 //for CLI(no use sysmem in ld) and CNN use(have offset while using sysmem)
#ifndef EXT_RAM_APPDATA_START
#define EXT_RAM_APPDATA_START	0x20020800
#endif
#ifndef EXT_RAM_PATTERN_START
#define EXT_RAM_PATTERN_START	0x200B0000
#endif
#endif

#ifndef EXT_RAM_SIZE
#define EXT_RAM_SIZE		0x160000 		// 46*32KB
#ifndef EXT_RAM_APPDATA_SIZE
#ifdef OS_TX
#define EXT_RAM_APPDATA_SIZE	0xE6000			// For Case 1.2 :    - dccm : stack heap + StaticMemory.o,    - sys  : .data .bss + luts.o
#else
#define EXT_RAM_APPDATA_SIZE	0x50000			// For Case 1.2 :    - dccm : stack heap + StaticMemory.o,    - sys  : .data .bss + luts.o
#endif
#endif
#ifndef EXT_RAM_PATTERN_SIZE
#define EXT_RAM_PATTERN_SIZE	0xC0000	// WDMA2 - 300KB, WDMA3 - 450KB
#endif
#endif

#ifndef EXT_ROM_START
#define EXT_ROM_START	ONCHIP_FLASH_BASE
#endif

#ifndef EXT_ROM_SIZE
#define EXT_ROM_SIZE	ONCHIP_FLASH_SIZE
#endif

#ifndef REGION_ROM
#ifdef ARC_FEATURE_ICACHE_PRESENT
#define REGION_ROM	EXT_RAM
#else
#define REGION_ROM	ICCM
#endif
#endif

#ifndef REGION_RAM
#ifdef ARC_FEATURE_DCACHE_PRESENT
#define REGION_RAM	EXT_RAM
#else
#define REGION_RAM	DCCM
#endif
#endif


#ifdef FLASH_AS_SRAM
	#define XIP_SSI0_ADDR 0x28000000
	#define XIP_SSI1_ADDR 0x30000000
	#ifdef IC_PACKAGE_WLCSP38
		#define FLASH_SECTOR_START (0x130000+XIP_SSI0_ADDR)
		#define XIP_SSI_ADDR XIP_SSI0_ADDR
	#else
		#if defined(AIOT_HUMANDETECT_AIRCONDITION) || defined(TFLITE_STANDALONE)
			#define FLASH_SECTOR_START (0x190000+XIP_SSI0_ADDR)
		#else
			#define FLASH_SECTOR_START (0x130000+XIP_SSI0_ADDR)
		#endif
		#define XIP_SSI_ADDR XIP_SSI0_ADDR
	#endif
#endif

#endif /* _TARGET_MEM_CONFIG_H_ */
