#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#include "Display.h"
#include "BspSpiDispIli9341.h"
#include "BspGpio.h"
#include "Gui.h"

static volatile bool isDisplayTxActive = false;

static void portDisplayCbControlCs(uint8_t state)
{
    if (state) {
        bspGpioSet(BSP_GPIO_OUT_DISP_ILI9341_CS);
    } else {
        bspGpioReset(BSP_GPIO_OUT_DISP_ILI9341_CS);
    }
}

static void portDisplayCbControlCxD(uint8_t state)
{
    if (state) {
        bspGpioSet(BSP_GPIO_OUT_DISP_ILI9341_CXD);
    } else {
        bspGpioReset(BSP_GPIO_OUT_DISP_ILI9341_CXD);
    }
}

static void portDisplayCbControlReset(uint8_t state)
{
    if (state) {
        bspGpioSet(BSP_GPIO_OUT_DISP_ILI9341_RESET);
    } else {
        bspGpioReset(BSP_GPIO_OUT_DISP_ILI9341_RESET);
    }
}

static void portDisplaySpiTxComplete(void)
{
    /*
     * Implement Unblock SPI communication  (Give RTOS semaphore or )
     */
    isDisplayTxActive = false;
}

static void portDisplayTxU8(uint8_t *data, uint32_t size)
{
    /*
     * Implement block SPI communication (take RTOS semaphore)
     */
    while(isDisplayTxActive) {
    }
    isDisplayTxActive = true;

    bspSpiDispIli9341TxU8(data, size);

    while(isDisplayTxActive) {
    }

    /*
     * Waite to complete previous transaction
     */
    bspSpiDisplayIli9341WaitBusy();
}

static void portDisplayTxU16(uint16_t *data, uint32_t size)
{
    /*
     * Implement block SPI communication (take RTOS semaphore)
     */
    while(isDisplayTxActive) {
    }
    isDisplayTxActive = true;

    bspSpiDispIli9341TxU16(data, size);

    while(isDisplayTxActive) {
    }

    /*
     * Waite to complete previous transaction
     */
    bspSpiDisplayIli9341WaitBusy();
}

__attribute__((optimize("O0")))
static void delayMs(uint32_t delay)
{
    delay *= 6000;
    while(delay--) {
    }
}

static uint8_t * guiMalloc(uint32_t *memSize)
{
#define GUI_MEM_SIZE    1024
    static uint8_t guiMemPool[GUI_MEM_SIZE];

    *memSize = GUI_MEM_SIZE;
    return guiMemPool;
}

static void guiSetRegion(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    __attribute__((unused))
    DisplayStatus result;

    result = displaySetArea(x, y, w, h);
}

static void guiPutPixels565(const GuiColor565 *buff, uint32_t cnt)
{
    __attribute__((unused))
    DisplayStatus result; 

    result = displayPutPixel565((MiddlewareDisplayColor565 *)buff, cnt);
}

static void guiPutPixels888(const GuiColor888 *buff, uint32_t cnt)
{
    __attribute__((unused))
    DisplayStatus result;
}

void portDisplayInit(void)
{
    DisplayCb middlewareDisplayCb = {
        .txU8 = portDisplayTxU8,
        .txU16 = portDisplayTxU16,
        .spiCs = portDisplayCbControlCs,
        .cxd = portDisplayCbControlCxD,
        .reset = portDisplayCbControlReset,
        .delayMs = delayMs,
    };
    BspSpiDispIli9341Cb spiDisplayCb = {
        .txComplete = portDisplaySpiTxComplete,
    };

    bspGpioSet(BSP_GPIO_OUT_DISP_ILI9341_CXD);
    bspGpioSet(BSP_GPIO_OUT_DISP_ILI9341_CS);
    bspGpioReset(BSP_GPIO_OUT_DISP_ILI9341_RESET);
    /*
     * It is important follow the initialization order
     * - communication SPI
     * - display middleware
     */
    bspSpiDispIli9341Init(spiDisplayCb);
    displayInit(middlewareDisplayCb);

    displaySetOrientation(DISPLAY_ORIENTATION_HORIZONTAL);

    /*
     * Init GUI interface
     */ 
    GuiCb guiCb = {
        .putPixels888 = guiPutPixels888,
        .putPixels565 = guiPutPixels565,
        .setRegion = guiSetRegion,
        .memAlloc = guiMalloc,
    };
    
    guiInit(guiCb, displayGetWidth(), displayGetHeight());
}