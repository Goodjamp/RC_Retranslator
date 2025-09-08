#ifndef __BSP_SERVICES_H__
#define __BSP_SERVICES_H__

#include <stdint.h>
#include <stdbool.h>

#include "stm32g4xx.h"
#include "stm32g4xx_ll_dma.h"

#define CONCAT_2_WORDS_(X,Y)            X##Y
#define CONCAT_2_WORDS(X,Y)             CONCAT_2_WORDS_(X,Y)

#define CONCAT_3_WORDS_(X,Y,Z)          X##Y##Z
#define CONCAT_3_WORDS(X,Y,Z)           CONCAT_3_WORDS_(X,Y,Z)

#define CONCAT_4_WORDS_(X,Y,Z,L)        X##Y##Z##L
#define CONCAT_4_WORDS(X,Y,Z,L)         CONCAT_4_WORDS_(X,Y,Z,L)

#define CONCAT_5_WORDS_(X,Y,Z,L,M)      X##Y##Z##L##M
#define CONCAT_5_WORDS(X,Y,Z,L,M)       CONCAT_5_WORDS_(X,Y,Z,L,M)

#define CONCAT_6_WORDS_(X,Y,Z,L,M,N)    X##Y##Z##L##M##N
#define CONCAT_6_WORDS(X,Y,Z,L,M,N)     CONCAT_6_WORDS_(X,Y,Z,L,M,N)

#define ARRAY_SIZE(array)               (sizeof(array) / sizeof(array[0]))
#define HAL_CALL_CB(cb, ...)            if (cb) {cb(__VA_ARGS__);}

bool bspServicesEnablePeriphery(void *pereph);
bool bspServicesDisablePerephr(void *pereph);
bool bspServicesSetResetPerephr(void *pereph);
bool bspServicesClearResetPerephr(void *pereph);
uint32_t bspServicesGetTimOscFrq(void *baseTim);

/**bspServicesDmaClearFlagTE
 * DMA transfer error clear flag
 */
void bspServicesDmaClearFlagTE(DMA_TypeDef *dma, uint32_t stream);

/**
 * DMA half transfer clear flag
 */
void bspServicesDmaClearFlagHT(DMA_TypeDef *dma, uint32_t stream);

/**
 * DMA direct mode error clear flag
 */
void bspServicesDmaClearFlagDME(DMA_TypeDef *dma, uint32_t stream);

/**
 * DMA FIFO error clear flag
 */
void bspServicesDmaClearFlagFE(DMA_TypeDef *dma, uint32_t stream);

/**
 * DMA transmit complete clear flag
 */
void bspServicesDmaClearFlagTC(DMA_TypeDef *dma, uint32_t stream);

/**
 * Return different than 0 if TC flag active
 */
uint32_t bspServicesDmaIsActiveFlagTC(DMA_TypeDef *dma, uint32_t stream);

#endif // __BSP_SERVICES_H__
