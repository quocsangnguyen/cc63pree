/*
 * Copyright (C) 2012-2017 ARM Limited or its affiliates.
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

/* \file buffer_mgr.h
   Buffer Manager
 */

#ifndef __BUFFER_MGR_H__
#define __BUFFER_MGR_H__

#include <crypto/algapi.h>

#include "dx_config.h"
#include "dx_driver.h"


enum dx_secure_dir_type {
	DX_NO_DMA_IS_SECURE = 0,
	DX_SRC_DMA_IS_SECURE = 1,
	DX_DST_DMA_IS_SECURE = 2
};

enum dx_req_dma_buf_type {
	DX_DMA_BUF_NULL = 0,
	DX_DMA_BUF_DLLI,
	DX_DMA_BUF_MLLI
};

enum dx_sg_cpy_direct {
	DX_SG_TO_BUF = 0,
	DX_SG_FROM_BUF = 1
};

struct dx_mlli {
	dx_sram_addr_t sram_addr;
	unsigned int nents; //sg nents
	unsigned int mlli_nents; //mlli nents might be different than the above
};

struct mlli_params {
	struct dma_pool *curr_pool;
	uint8_t *mlli_virt_addr;
	dma_addr_t mlli_dma_addr;
	uint32_t mlli_len;  
};

int dx_buffer_mgr_init(struct dx_drvdata *drvdata);

int dx_buffer_mgr_fini(struct dx_drvdata *drvdata);

int dx_buffer_mgr_map_ablkcipher_request(struct dx_drvdata *drvdata, struct ablkcipher_request *req);

void dx_buffer_mgr_unmap_ablkcipher_request(struct device *dev, struct ablkcipher_request *req);

int dx_buffer_mgr_map_aead_request(struct dx_drvdata *drvdata, struct aead_request *req);

void dx_buffer_mgr_unmap_aead_request(struct device *dev, struct aead_request *req);

int dx_buffer_mgr_map_ahash_request_final(struct dx_drvdata *drvdata, struct ahash_request *req, bool do_update);

int dx_buffer_mgr_map_ahash_request_update(struct dx_drvdata *drvdata, struct ahash_request *req);

void dx_buffer_mgr_unmap_ahash_request(struct device *dev, struct ahash_request *req, bool do_revert);

void dx_buffer_mgr_copy_scatterlist_portion(u8 *dest, struct scatterlist *sg, uint32_t to_skip, uint32_t end, enum dx_sg_cpy_direct direct);



#ifdef DX_DMA_48BIT_SIM
dma_addr_t dx_buff_mgr_update_dma_addr(dma_addr_t orig_addr, uint32_t data_len);
dma_addr_t dx_buff_mgr_restore_dma_addr(dma_addr_t orig_addr);

#define DX_UPDATE_DMA_ADDR_TO_48BIT(addr,size) addr = \
					dx_buff_mgr_update_dma_addr(addr,size)
#define DX_RESTORE_DMA_ADDR_TO_48BIT(addr) addr = \
					dx_buff_mgr_restore_dma_addr(addr)
#else

#define DX_UPDATE_DMA_ADDR_TO_48BIT(addr,size) addr = addr
#define DX_RESTORE_DMA_ADDR_TO_48BIT(addr) addr = addr

#endif

#endif /*__BUFFER_MGR_H__*/

