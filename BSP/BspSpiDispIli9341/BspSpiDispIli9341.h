#ifndef __SPI_DISPLAY_ILI9341_H__
#define __SPI_DISPLAY_ILI9341_H__

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief This file provide ILI9341 low layer communication API
 */

typedef enum {
    BSP_SPI_DISP_ILI9341_RES_OK,
    BSP_SPI_DISP_ILI9341_RES_HW_ERROR,
    BSP_SPI_DISP_ILI9341_RES_BUFF_NULL_ERROR,
    BSP_SPI_DISP_ILI9341_RES_SIZE_0_ERROR,
    SPI_DISP_ILI9341_RES_INTERNAL_ERROR,
} BspSpiDispIli9341Res;

typedef struct {
    void (*txComplete)(void);
} BspSpiDispIli9341Cb;

BspSpiDispIli9341Res bspSpiDispIli9341Init(BspSpiDispIli9341Cb cb);
bool bspSpiDisplayIli9341WaitBusy(void);
BspSpiDispIli9341Res bspSpiDispIli9341TxU8(uint8_t buff[], uint32_t size);
BspSpiDispIli9341Res bspSpiDispIli9341TxU16(uint16_t buff[], uint32_t size);

#endif
