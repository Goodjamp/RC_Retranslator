#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "BspSpiDispIli9341.h"
#include "stm32g4xx_ll_spi.h"
#include "stm32g4xx_ll_gpio.h"
#include "stm32g4xx_ll_dma.h"
#include "stm32g4xx.h"

#include "BSP.h"
#include "BspDebug.h"
#include "BspServices.h"
#include "BspGpio.h"

/*
 * The counter below provides a timeout for a 16 + 1 bit delay.
 * This is necessary because the DMA transmit-complete interrupt occurs before the SPI finishes transmitting the last 2 bytes.
 */
#define SPI_DISPLAY_ILI9341_BSY_WAITE    40000

static BspSpiDispIli9341Cb callbacks;
const static struct {
    SPI_TypeDef *spi;
    DMA_TypeDef *dma;
    uint32_t dmaChannel;
    uint32_t dmaReqSrc;
    struct {
        GPIO_TypeDef *port;
        uint32_t pin;
        uint32_t  alt_fnc;      
    } gpio[2];
} settings = {
    .spi = CONCAT_2_WORDS(SPI, SPI_DISPLAY_ILI9341),
    .dma = CONCAT_2_WORDS(DMA, DMA_DISPLAY_ILI9341),
    .dmaChannel = CONCAT_2_WORDS(LL_DMA_CHANNEL_, DMA_DISPLAY_ILI9341_CH),
    .dmaReqSrc = CONCAT_3_WORDS(LL_DMAMUX_REQ_SPI, SPI_DISPLAY_ILI9341,_TX),
    .gpio = {
        {CONCAT_2_WORDS(GPIO, GPIO_DISP_ILI9341_SPI_SCK_PORT), CONCAT_2_WORDS(LL_GPIO_PIN_, GPIO_DISP_ILI9341_SPI_SCK_PIN), CONCAT_2_WORDS(LL_GPIO_AF_, GPIO_DISP_ILI9341_SPI_SCK_ALT_FUN)},
        {CONCAT_2_WORDS(GPIO, GPIO_DISP_ILI9341_SPI_MOSI_PORT), CONCAT_2_WORDS(LL_GPIO_PIN_, GPIO_DISP_ILI9341_SPI_MOSI_PIN), CONCAT_2_WORDS(LL_GPIO_AF_, GPIO_DISP_ILI9341_SPI_MOSI_ALT_FUN)},
    }
};

#define dmaDisplayIli9341IrqH    CONCAT_5_WORDS(DMA, DMA_DISPLAY_ILI9341, _Channel, DMA_DISPLAY_ILI9341_CH, _IRQHandler)
#define dmaDisplayIli9341IrqN    CONCAT_5_WORDS(DMA, DMA_DISPLAY_ILI9341, _Channel, DMA_DISPLAY_ILI9341_CH, _IRQn)

/*
 * DMA transaction complete
 */
void dmaDisplayIli9341IrqH(void)
{
    LL_DMA_DisableIT_TC(settings.dma, settings.dmaChannel);
    LL_SPI_DisableDMAReq_TX(settings.spi);
    LL_DMA_DisableChannel(settings.dma, settings.dmaChannel);

    bspServicesDmaClearFlagTC(settings.dma, settings.dmaChannel);
    NVIC_ClearPendingIRQ(dmaDisplayIli9341IrqN);

    if (callbacks.txComplete != NULL) {
        callbacks.txComplete();
    }
}

static void spiDisplayIli9341InitGpio(void)
{
    LL_GPIO_InitTypeDef gpioInitStruct = {
        .Alternate = LL_GPIO_AF_5,
        .Mode = LL_GPIO_MODE_ALTERNATE,
        .Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH,
        .OutputType = LL_GPIO_OUTPUT_PUSHPULL,
        .Pull = LL_GPIO_PULL_NO,
    };

    for (uint32_t k = 0; k < ARRAY_SIZE(settings.gpio); k++) {
        gpioInitStruct.Pin = settings.gpio[k].pin;
        gpioInitStruct.Alternate = settings.gpio[k].alt_fnc;
        bspServicesEnablePeriphery(settings.gpio[k].port);
        LL_GPIO_Init(settings.gpio[k].port, &gpioInitStruct);
    }
}

static void spiDisplayIli9341InitDma(void)
{
    bspServicesEnablePeriphery(settings.dma);
    bspServicesEnablePeriphery(DMAMUX1);

    LL_DMA_InitTypeDef dmaInit = {
        .NbData = 0,
        .MemoryOrM2MDstAddress = 0,
        .PeriphOrM2MSrcAddress = (uint32_t)&settings.spi->DR,
        .Mode = LL_DMA_MODE_NORMAL,
        .MemoryOrM2MDstIncMode = LL_DMA_MEMORY_INCREMENT,
        .PeriphOrM2MSrcIncMode = LL_DMA_PERIPH_NOINCREMENT,
        .PeriphOrM2MSrcDataSize = LL_DMA_PDATAALIGN_BYTE,
        .MemoryOrM2MDstDataSize = LL_DMA_MDATAALIGN_BYTE,
        .Priority = LL_DMA_PRIORITY_LOW,
        .Direction = LL_DMA_DIRECTION_MEMORY_TO_PERIPH,
        .PeriphRequest = settings.dmaReqSrc,
    };

    LL_DMA_Init(settings.dma, settings.dmaChannel, &dmaInit);
    LL_DMA_DisableChannel(settings.dma, settings.dmaChannel);

    NVIC_SetPriority(dmaDisplayIli9341IrqN, 6);
    NVIC_EnableIRQ(dmaDisplayIli9341IrqN);
}

static void spiDisplayIli9341InitSpi(void)
{
    bspServicesEnablePeriphery(settings.spi);

    LL_SPI_InitTypeDef spiInit = {
        .TransferDirection = LL_SPI_HALF_DUPLEX_TX,
        .Mode = LL_SPI_MODE_MASTER,
        .DataWidth = LL_SPI_DATAWIDTH_8BIT,
        .ClockPolarity = LL_SPI_POLARITY_LOW,
        .ClockPhase = LL_SPI_PHASE_1EDGE,
        .BaudRate = LL_SPI_BAUDRATEPRESCALER_DIV8,
        .BitOrder = LL_SPI_MSB_FIRST,
        .CRCCalculation = LL_SPI_CRCCALCULATION_DISABLE,
    };

    spiInit.NSS = LL_SPI_NSS_SOFT;
    LL_SPI_Init(settings.spi, &spiInit);
    LL_SPI_SetStandard(settings.spi, LL_SPI_PROTOCOL_MOTOROLA);
    LL_SPI_DisableDMAReq_TX(settings.spi);

    LL_SPI_Enable(settings.spi);
}

void bspSpiDispIli9341SetSpiBaudRatePrescaler(uint32_t BaudRate)
{
    LL_SPI_SetBaudRatePrescaler(settings.spi, BaudRate);
}

BspSpiDispIli9341Res bspSpiDispIli9341Init(BspSpiDispIli9341Cb cb)
{
    callbacks = cb;

    spiDisplayIli9341InitGpio();
    spiDisplayIli9341InitDma();
    spiDisplayIli9341InitSpi();
    bspGpioSet(BSP_GPIO_OUT_DISP_ILI9341_LED);

    return BSP_SPI_DISP_ILI9341_RES_OK;
}

//__attribute__((optimize("O0")))
bool bspSpiDisplayIli9341WaitBusy(void)
{
    volatile uint32_t waitBsyCnt = SPI_DISPLAY_ILI9341_BSY_WAITE;

    /*
     * Waite to complete transaction
     */
    while (LL_SPI_IsActiveFlag_BSY(settings.spi) == 1
           && (--waitBsyCnt != 0)) {
    }

    return waitBsyCnt != 0;
}

BspSpiDispIli9341Res bspSpiDispIli9341Tx(uint32_t address, uint32_t size, bool isU8)
{
    if (address == 0x00){
        return BSP_SPI_DISP_ILI9341_RES_BUFF_NULL_ERROR;
    }
    if (size == 0){
        return BSP_SPI_DISP_ILI9341_RES_SIZE_0_ERROR;
    }

    if (bspSpiDisplayIli9341WaitBusy() == false) {
        return BSP_SPI_DISP_ILI9341_RES_HW_ERROR;
    }

    LL_DMA_DisableChannel(settings.dma, settings.dmaChannel);
    
    if (LL_SPI_IsActiveFlag_TXE(settings.spi) == false) {
        /*
        * To start the SPI communication if DR not empty (TxE clear) we need to set TxE i. e. read the DR.
        */
        LL_SPI_ReceiveData8(settings.spi);
    }
    LL_SPI_SetDataWidth(settings.spi, isU8 ? LL_SPI_DATAWIDTH_8BIT : LL_SPI_DATAWIDTH_16BIT);
    LL_SPI_EnableDMAReq_TX(settings.spi);

    bspServicesDmaClearFlagTC(settings.dma, settings.dmaChannel);
    bspServicesDmaClearFlagHT(settings.dma, settings.dmaChannel);
    //bspServicesDmaClearFlagTE(settings.dma, settings.dmaStream);
    //bspServicesDmaClearFlagDME(settings.dma, settings.dmaStream);
    //bspServicesDmaClearFlagFE(settings.dma, settings.dmaStream);

    LL_DMA_SetPeriphSize(settings.dma, settings.dmaChannel, isU8 ? LL_DMA_PDATAALIGN_BYTE : LL_DMA_PDATAALIGN_HALFWORD);
    LL_DMA_SetMemorySize(settings.dma, settings.dmaChannel, isU8 ? LL_DMA_MDATAALIGN_BYTE : LL_DMA_MDATAALIGN_HALFWORD);
    LL_DMA_SetMemoryAddress(settings.dma, settings.dmaChannel, address);
    LL_DMA_SetDataLength(settings.dma, settings.dmaChannel, size);
    LL_DMA_EnableChannel(settings.dma, settings.dmaChannel);
    /*
     * GCC: if optimisation is -O3 the code skeep LL_DMA_EnableIT_TC
     */
    __DSB();
    LL_DMA_EnableIT_TC(settings.dma, settings.dmaChannel);

    return BSP_SPI_DISP_ILI9341_RES_OK;
}

BspSpiDispIli9341Res bspSpiDispIli9341TxU8(uint8_t buff[], uint32_t size)
{
    return bspSpiDispIli9341Tx((uint32_t)buff, size, true);
}

BspSpiDispIli9341Res bspSpiDispIli9341TxU16(uint16_t buff[], uint32_t size)
{
    return bspSpiDispIli9341Tx((uint32_t)buff, size, false);
}
