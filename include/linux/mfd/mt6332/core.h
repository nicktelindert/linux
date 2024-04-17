/* SPDX-License-Identifier: GPL-2.0-only */
/*
 * Copyright (c) 2022 AngeloGioacchino Del Regno <angelogioacchino.delregno@collabora.com>
 */

#ifndef __MFD_MT6332_CORE_H__
#define __MFD_MT6332_CORE_H__

enum mt6332_irq_status_numbers {
	MT6332_IRQ_STATUS_CHR_COMPLETE = 0,
	MT6332_IRQ_STATUS_THERMAL_SD,
	MT6332_IRQ_STATUS_THERMAL_REG_IN,
	MT6332_IRQ_STATUS_THERMAL_REG_OUT,
	MT6332_IRQ_STATUS_OTG_OC,
	MT6332_IRQ_STATUS_CHR_OC,
	MT6332_IRQ_STATUS_OTG_THERMAL,
	MT6332_IRQ_STATUS_CHRIN_SHORT,
	MT6332_IRQ_STATUS_DRVCDT_SHORT,
	MT6332_IRQ_STATUS_PLUG_IN_FLASH,
	MT6332_IRQ_STATUS_CHRWDT_FLAG,
	MT6332_IRQ_STATUS_FLASH_EN_TIMEOUT,
	MT6332_IRQ_STATUS_FLASH_VLED1_SHORT,
	MT6332_IRQ_STATUS_FLASH_VLED1_OPEN = 13,
	MT6332_IRQ_STATUS_OV = 16,
	MT6332_IRQ_STATUS_BVALID_DET,
	MT6332_IRQ_STATUS_VBATON_UNDET,
	MT6332_IRQ_STATUS_CHR_PLUG_IN,
	MT6332_IRQ_STATUS_CHR_PLUG_OUT,
	MT6332_IRQ_STATUS_BC11_TIMEOUT,
	MT6332_IRQ_STATUS_FLASH_VLED2_SHORT,
	MT6332_IRQ_STATUS_FLASH_VLED2_OPEN = 23,
	MT6332_IRQ_STATUS_THR_H = 32,
	MT6332_IRQ_STATUS_THR_L,
	MT6332_IRQ_STATUS_BAT_H,
	MT6332_IRQ_STATUS_BAT_L,
	MT6332_IRQ_STATUS_M3_H,
	MT6332_IRQ_STATUS_M3_L,
	MT6332_IRQ_STATUS_FG_BAT_H,
	MT6332_IRQ_STATUS_FG_BAT_L,
	MT6332_IRQ_STATUS_FG_CUR_H,
	MT6332_IRQ_STATUS_FG_CUR_L,
	MT6332_IRQ_STATUS_SPKL_D,
	MT6332_IRQ_STATUS_SPKL_AB,
	MT6332_IRQ_STATUS_BIF,
	MT6332_IRQ_STATUS_VWLED_OC = 45,
	MT6332_IRQ_STATUS_VDRAM_OC = 48,
	MT6332_IRQ_STATUS_VDVFS2_OC,
	MT6332_IRQ_STATUS_VRF1_OC,
	MT6332_IRQ_STATUS_VRF2_OC,
	MT6332_IRQ_STATUS_VPA_OC,
	MT6332_IRQ_STATUS_VSBST_OC,
	MT6332_IRQ_STATUS_LDO_OC,
	MT6332_IRQ_STATUS_NR,
};

#define MT6332_IRQ_CON0_BASE	MT6332_IRQ_STATUS_CHR_COMPLETE
#define MT6332_IRQ_CON0_BITS	(MT6332_IRQ_STATUS_FLASH_VLED1_OPEN + 1)
#define MT6332_IRQ_CON1_BASE	MT6332_IRQ_STATUS_OV
#define MT6332_IRQ_CON1_BITS	(MT6332_IRQ_STATUS_FLASH_VLED2_OPEN - MT6332_IRQ_STATUS_OV + 1)
#define MT6332_IRQ_CON2_BASE	MT6332_IRQ_STATUS_THR_H
#define MT6332_IRQ_CON2_BITS	(MT6332_IRQ_STATUS_VWLED_OC - MT6332_IRQ_STATUS_THR_H + 1)
#define MT6332_IRQ_CON3_BASE	MT6332_IRQ_STATUS_VDRAM_OC
#define MT6332_IRQ_CON3_BITS	(MT6332_IRQ_STATUS_LDO_OC - MT6332_IRQ_STATUS_VDRAM_OC + 1)

#endif /* __MFD_MT6332_CORE_H__ */
