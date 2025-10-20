#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "Display.h"

/*
 * The code bellow implement the ILI9341 display
 */

#define FILL_ARRAY_1(X, X0)                                         X[0] = (X0)
#define FILL_ARRAY_2(X, X0, X1)                                     X[0] = (X0); X[1] = (X1)
#define FILL_ARRAY_3(X, X0, X1, X2)                                 X[0] = (X0); X[1] = (X1); X[2] = (X2)
#define FILL_ARRAY_4(X, X0, X1, X2, X3)                             X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = X3
#define FILL_ARRAY_5(X, X0, X1, X2, X3, X4)                         X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = (X3); X[4] = X4
#define FILL_ARRAY_6(X, X0, X1, X2, X3, X4, X5)                     X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = (X3); X[4] = (X4); X[5] = X5
#define FILL_ARRAY_7(X, X0, X1, X2, X3, X4, X5, X6)                 X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = (X3); X[4] = (X4); X[5] = (X5);\
X[6] = X6
#define FILL_ARRAY_8(X, X0, X1, X2, X3, X4, X5, X6, X7)             X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = (X3); X[4] = (X4); X[5] = (X5);\
X[6] = (X6); X[7] = X7
#define FILL_ARRAY_9(X, X0, X1, X2, X3, X4, X5, X6, X7, X8)         X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = (X3); X[4] = (X4); X[5] = (X5);\
X[6] = (X6); X[7] = (X7); X[8] = X8
#define FILL_ARRAY_10(X, X0, X1, X2, X3, X4, X5, X6, X7, X8, X9)    X[0] = (X0); X[1] = (X1); X[2] = (X2); X[3] = (X3); X[4] = (X4); X[5] = (X5);\
X[6] = (X6); X[7] = (X7); X[8] = (X8); X[9] = (X9)

#define ILI9341_SET_MASK(X, Y)       (X |= (Y))
#define ILI9341_CLEAR_MASK(X, Y)     (X &= ~(Y))

#define DISPLAY_IS_ORIENTATION_VERTICAL    (displayHeight > displayWidth)

typedef enum {
    DISPLAY_ILI9341_COMMAND_NOP                  = 0x00, // No operation
    DISPLAY_ILI9341_COMMAND_SOFTWARE_RESET       = 0x01, // Software reset
    DISPLAY_ILI9341_COMMAND_READ_DISPLAY_ID      = 0x04, // Read display ID
    DISPLAY_ILI9341_COMMAND_READ_DISPLAY_STATUS  = 0x09, // Read display status
    DISPLAY_ILI9341_COMMAND_READ_POWER_MODE      = 0x0A, // Read power mode
    DISPLAY_ILI9341_COMMAND_READ_MADCTL          = 0x0B, // Read memory access control
    DISPLAY_ILI9341_COMMAND_READ_PIXEL_FORMAT    = 0x0C, // Read pixel format
    DISPLAY_ILI9341_COMMAND_READ_IMAGE_FORMAT    = 0x0D, // Read image format
    DISPLAY_ILI9341_COMMAND_READ_SIGNAL_MODE     = 0x0E, // Read signal mode
    DISPLAY_ILI9341_COMMAND_SLEEP_IN             = 0x10, // Enter sleep mode
    DISPLAY_ILI9341_COMMAND_SLEEP_OUT            = 0x11, // Exit sleep mode
    DISPLAY_ILI9341_COMMAND_PARTIAL_MODE_ON      = 0x12, // Enable partial mode
    DISPLAY_ILI9341_COMMAND_NORMAL_DISPLAY_ON    = 0x13, // Enable normal display
    DISPLAY_ILI9341_COMMAND_INVERSION_OFF        = 0x20, // Display inversion OFF
    DISPLAY_ILI9341_COMMAND_INVERSION_ON         = 0x21, // Display inversion ON
    DISPLAY_ILI9341_COMMAND_GAMMA_SET            = 0x26, // Set gamma curve
    DISPLAY_ILI9341_COMMAND_DISPLAY_OFF          = 0x28, // Turn display OFF
    DISPLAY_ILI9341_COMMAND_DISPLAY_ON           = 0x29, // Turn display ON
    DISPLAY_ILI9341_COMMAND_COLUMN_ADDRESS_SET   = 0x2A, // Set column address range
    DISPLAY_ILI9341_COMMAND_PAGE_ADDRESS_SET     = 0x2B, // Set page address range
    DISPLAY_ILI9341_COMMAND_MEMORY_WRITE         = 0x2C, // Write memory
    DISPLAY_ILI9341_COMMAND_COLOR_SET            = 0x2D, // Initiate memory color
    DISPLAY_ILI9341_COMMAND_MEMORY_READ          = 0x2E, // Read memory
    DISPLAY_ILI9341_COMMAND_PARTIAL_AREA         = 0x30, // Define partial window
    DISPLAY_ILI9341_COMMAND_VERT_SCROLL_DEFINITION = 0x33, // Define vertical scroll
    DISPLAY_ILI9341_COMMAND_MEMORY_ACCESS_CTRL   = 0x36, // Set memory access mode
    DISPLAY_ILI9341_COMMAND_VERT_SCROLL_START_ADDRESS = 0x37, // Vertical scroll start
    DISPLAY_ILI9341_COMMAND_PIXEL_FORMAT_SET     = 0x3A, // Set pixel format
    DISPLAY_ILI9341_COMMAND_MEMORY_WRITE_CONTINUE = 0x3C, // Set pixel format
    DISPLAY_ILI9341_COMMAND_RAM_WRITE_DATA_MASK_CTRL = 0x3E, // RAM write data mask
    DISPLAY_ILI9341_COMMAND_FRAME_RATE_CTRL1     = 0xB1, // Frame rate control (normal)
    DISPLAY_ILI9341_COMMAND_FRAME_RATE_CTRL2     = 0xB2, // Frame rate control (idle)
    DISPLAY_ILI9341_COMMAND_FRAME_RATE_CTRL3     = 0xB3, // Frame rate control (partial)
    DISPLAY_ILI9341_COMMAND_DISPLAY_INVERSION_CTRL = 0xB4, // Display inversion control
    DISPLAY_ILI9341_COMMAND_DOTCLOCK_FREQ_CTRL   = 0xB6, // Dot clock frequency control
    DISPLAY_ILI9341_COMMAND_POWER_CTRL1          = 0xC0, // Power control 1
    DISPLAY_ILI9341_COMMAND_POWER_CTRL2          = 0xC1, // Power control 2
    DISPLAY_ILI9341_COMMAND_VCOM_CTRL1           = 0xC5, // VCOM control 1
    DISPLAY_ILI9341_COMMAND_VCOM_CTRL2           = 0xC7, // VCOM control 2
    DISPLAY_ILI9341_COMMAND_POSITIVE_GAMMA_CORRECTION = 0xE0, // Positive gamma correction
    DISPLAY_ILI9341_COMMAND_NEGATIVE_GAMMA_CORRECTION = 0xE1, // Negative gamma correction
    DISPLAY_ILI9341_COMMAND_GAMMA_PLUS_PARAM      = 0xF2, // Gamma + Parameter control
    DISPLAY_ILI9341_COMMAND_INTERFACE_CTRL        = 0xF6, // Interface control
} DisplayIli9341Command;

/****************************/
/******    MADCTL   *********/
/****************************/
#define ILI9341_DEF_MADCTL           0x00U
#define ILI9341_OFFSET_MADCTL_MY     0x7U
#define ILI9341_MASK_MADCTL_MY       (1 << ILI9341_OFFSET_MADCTL_MY)
#define ILI9341_OFFSET_MADCTL_MX     0x6U
#define ILI9341_MASK_MADCTL_MX       (1 << ILI9341_OFFSET_MADCTL_MX)
#define ILI9341_OFFSET_MADCTL_MV     0x5U
#define ILI9341_MASK_MADCTL_MV       (1 << ILI9341_OFFSET_MADCTL_MV)
#define ILI9341_OFFSET_MADCTL_ML     0x4U
#define ILI9341_MASK_MADCTL_ML       (1 << ILI9341_OFFSET_MADCTL_ML)
#define ILI9341_OFFSET_MADCTL_BGR    0x3U
#define ILI9341_MASK_MADCTL_BGR      (1 << ILI9341_OFFSET_MADCTL_BGR)
#define ILI9341_OFFSET_MADCTL_MH     0x2U
#define ILI9341_MASK_MADCTL_MH       (1 << ILI9341_OFFSET_MADCTL_MH)

/**
 * The DIMENSION definition is used to set WIDTH and HEIGHT of the display
 * depends on the orientation:
 * - DISPLAY_ORIENTATION_VERTICAL:   WIDTH = DISPLAY_DIMENSION_LESS (X), HEIGHT = DISPLAY_DIMENSION_MACH (Y)
 * - DISPLAY_ORIENTATION_HORIZONTAL: WIDTH = DISPLAY_DIMENSION_MACH (X), HEIGHT = DISPLAY_DIMENSION_LESS (Y)
 * See function *displaySetOrientation*
 */
#define DISPLAY_DIMENSION_LESS    240
#define DISPLAY_DIMENSION_MACH    320

static DisplayCb displayCb;
static bool isDisplayInit = false;
static bool isAreaUpdate = false;
static uint32_t displayWidth = DISPLAY_DIMENSION_LESS; // max X coord
static uint32_t displayHeight = DISPLAY_DIMENSION_MACH; // max Y coord

static void displaySendCommand(uint8_t command)
{
    displayCb.spiCs(0);
    displayCb.cxd(0);
    displayCb.txU8(&command, 1);
    displayCb.spiCs(1);
    displayCb.cxd(1);
}

static void displaySendData(uint8_t *data, uint32_t size)
{
    displayCb.spiCs(0);
    displayCb.cxd(1);
    displayCb.txU8(data, size);
    displayCb.spiCs(1);
    displayCb.cxd(1);
}

static void displaySendImage(uint16_t *data, uint32_t size)
{
    displayCb.spiCs(0);
    displayCb.cxd(1);
    displayCb.txU16(data, size);
    displayCb.spiCs(1);
    displayCb.cxd(1);
}

static void displayReset(void)
{
    displayCb.reset(0);
    displayCb.delayMs(4);
    displayCb.reset(1);
}

static void displayInitSequency(void)
{
    uint8_t txBuff[100];

    displayReset();
    //soft reset
    displaySendCommand(DISPLAY_ILI9341_COMMAND_SOFTWARE_RESET);
    displayCb.delayMs(1000);

    //power control A
    displaySendCommand(0xCB);
    FILL_ARRAY_5(txBuff, 0x39, 0x2C, 0x00, 0x34, 0x02);
    displaySendData(txBuff, 5);

    //power control B
    displaySendCommand(0xCF);
    FILL_ARRAY_3(txBuff, 0x00, 0xC1, 0x30);
    displaySendData(txBuff, 3);

    //driver timing control A
    displaySendCommand(0xE8);
    FILL_ARRAY_3(txBuff, 0x85, 0x00, 0x78);
    displaySendData(txBuff, 3);

    //driver timing control B
    displaySendCommand(0xEA);
    FILL_ARRAY_2(txBuff, 0x00, 0x00);
    displaySendData(txBuff, 2);


    //power on sequence control
    displaySendCommand(0xED);
    FILL_ARRAY_4(txBuff, 0x64, 0x03, 0x12, 0x81);
    displaySendData(txBuff, 4);

    //pump ratio control
    displaySendCommand(0xF7);
    FILL_ARRAY_1(txBuff, 0x20);
    displaySendData(txBuff, 1);

    //power control,VRH[5:0]
    displaySendCommand(0xC0);
    FILL_ARRAY_1(txBuff, 0x23);
    displaySendData(txBuff, 1);

    //Power control,SAP[2:0];BT[3:0]
    displaySendCommand(0xC1);
    FILL_ARRAY_1(txBuff, 0x10);
    displaySendData(txBuff, 1);

    //vcm control
    displaySendCommand(0xC5);
    FILL_ARRAY_2(txBuff, 0x3E, 0x28);
    displaySendData(txBuff, 2);

    //vcm control 2
    displaySendCommand(0xC7);
    FILL_ARRAY_1(txBuff, 0x86);
    displaySendData(txBuff, 1);

    //memory access control
    displaySendCommand(0x36);
    FILL_ARRAY_1(txBuff, 0x48);
    displaySendData(txBuff, 1);

    //pixel format
    displaySendCommand(0x3A);
    FILL_ARRAY_1(txBuff, 0x55);
    displaySendData(txBuff, 1);

    //frameration control,normal mode full colours
    displaySendCommand(DISPLAY_ILI9341_COMMAND_FRAME_RATE_CTRL1);
    FILL_ARRAY_2(txBuff, 0x00, 0x18);
    displaySendData(txBuff, 2);

    //display function control
    displaySendCommand(0xB6);
    FILL_ARRAY_3(txBuff, 0x08, 0x82, 0x27);
    displaySendData(txBuff, 3);

    //3gamma function disable
    displaySendCommand(0xF2);
    FILL_ARRAY_1(txBuff, 0x00);
    displaySendData(txBuff, 1);

    //gamma curve selected
    displaySendCommand(0x26);
    FILL_ARRAY_1(txBuff, 0x01);
    displaySendData(txBuff, 1);


    //set positive gamma correction
    displaySendCommand(0xE0);
    FILL_ARRAY_10(txBuff, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, 0x4E, 0xF1, 0x37, 0x07);
    txBuff[10] = 0x10;
    txBuff[11] = 0x03;
    txBuff[12] = 0x0E;
    txBuff[13] = 0x09;
    txBuff[14] = 0x00;
    displaySendData(txBuff, 15);

    //set negative gamma correction
    displaySendCommand(0xE1);
    FILL_ARRAY_10(txBuff, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, 0x31, 0xC1, 0x48, 0x08);
    txBuff[10] = 0x0F;
    txBuff[11] = 0x0C;
    txBuff[12] = 0x31;
    txBuff[13] = 0x36;
    txBuff[14] = 0x0F;
    displaySendData(txBuff, 15);

    //exit sleep
    displaySendCommand(0x11);
    displayCb.delayMs(120);

    //display on
    displaySendCommand(0x29);
}

static void middlewareDisplaySet16Bitcolor(void)
{
    uint8_t data[1] = {0};

    data[0] = 0xFF & ((5u << 4) | 5u);
    displaySendCommand(DISPLAY_ILI9341_COMMAND_PIXEL_FORMAT_SET);
    displaySendData(data, 1);
}

DisplayStatus displayInit(DisplayCb cb)
{
    displayCb = cb;
    if (displayCb .cxd == NULL
        || displayCb .spiCs == NULL
        || displayCb .reset == NULL
        || displayCb .txU8 == NULL
        || displayCb .txU16 == NULL) {
        return DISPLAY_STATUS_ERROR_CB;
    }
    
    displayInitSequency();
    middlewareDisplaySet16Bitcolor();
    isDisplayInit = true;
    return DISPLAY_STATUS_OK;
}

DisplayStatus displayPutPixel565(MiddlewareDisplayColor565 *colorBuff, uint32_t pixelCnt)
{
    if (colorBuff == NULL) {
        return DISPLAY_STATUS_ERROR_TX_NULL;
    }

    displaySendCommand(isAreaUpdate
                                 ? DISPLAY_ILI9341_COMMAND_MEMORY_WRITE
                                 : DISPLAY_ILI9341_COMMAND_MEMORY_WRITE_CONTINUE);
    displaySendImage((uint16_t *)colorBuff, pixelCnt);
    isAreaUpdate = false;

    return DISPLAY_STATUS_OK;
}

DisplayStatus displaySetArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height)
{
    uint32_t x2 = x + width - 1;
    uint32_t y2 = y + height - 1;

    if (x > displayWidth) {
        return DISPLAY_STATUS_ERROR_X_OUT_OF_RANGE;
    }

    if (x2 > displayWidth) {
        return DISPLAY_STATUS_ERROR_WIDTH_OUT_OF_RANGE;
    }

    if (y > displayHeight) {
        return DISPLAY_STATUS_ERROR_Y_OUT_OF_RANGE;
    }

    if (y2 > displayHeight) {
        return DISPLAY_STATUS_ERROR_HEIGHT_OUT_OF_RANGE;
    }
   
    uint8_t data[4] = {0xFF & (y >> 8) , 0xFF & (y), 0xFF & (y2 >> 8) , 0xFF & (y2)};
    displaySendCommand(DISPLAY_ILI9341_COMMAND_PAGE_ADDRESS_SET);
    displaySendData(data, sizeof(data));

    FILL_ARRAY_4(data, 0xFF & (x >> 8), 0xFF & (x), 0xFF & (x2 >> 8) , 0xFF & (x2));
    displaySendCommand(DISPLAY_ILI9341_COMMAND_COLUMN_ADDRESS_SET);
    displaySendData(data, sizeof(data));

    isAreaUpdate = true;

/*
    FILL_ARRAY_4(data, 0xFF & (y >> 8) , 0xFF & (y), 0xFF & (y2 >> 8) , 0xFF & (y2));
    displaySendCommand(DISPLAY_ILI9341_COMMAND_PAGE_ADDRESS_SET);
    displaySendData(data, sizeof(data));

    uint8_t data[4] = {0xFF & (x >> 8) , 0xFF & (x), 0xFF & (x2 >> 8) , 0xFF & (x2)};
    displaySendCommand(DISPLAY_ILI9341_COMMAND_COLUMN_ADDRESS_SET);
    displaySendData(data, sizeof(data));
*/
    return DISPLAY_STATUS_OK;
}

DisplayStatus displaySetOrientation(DisplayOrientation orientation)
{
    if (orientation >= DISPLAY_ORIENTATION_CNT) {
        return DISPLAY_STATUS_ERROR_ORIENTATION_OUT_OF_RANGE;
    }
    /*
     * Set logical orientation 
     */
    displayWidth = (orientation == DISPLAY_ORIENTATION_VERTICAL)
                   ? DISPLAY_DIMENSION_LESS : DISPLAY_DIMENSION_MACH;
    displayHeight = (orientation == DISPLAY_ORIENTATION_VERTICAL)
                   ? DISPLAY_DIMENSION_MACH : DISPLAY_DIMENSION_LESS;
    
    /*
     * Set physical display RAM presentation 
     */
    uint8_t settings[1];
    FILL_ARRAY_1(settings, ILI9341_DEF_MADCTL);
    if (orientation == DISPLAY_ORIENTATION_HORIZONTAL) {
        ILI9341_SET_MASK(settings[0], ILI9341_MASK_MADCTL_MV);
    }

    /*
     * Set BGR color order by the default
     */
    ILI9341_SET_MASK(settings[0], ILI9341_MASK_MADCTL_BGR);

    displaySendCommand(DISPLAY_ILI9341_COMMAND_MEMORY_ACCESS_CTRL);
    displaySendData(settings, sizeof(settings));

    return DISPLAY_STATUS_OK;
}

uint32_t displayGetWidth(void)
{
    return DISPLAY_IS_ORIENTATION_VERTICAL ? DISPLAY_DIMENSION_LESS : DISPLAY_DIMENSION_MACH;
}

uint32_t displayGetHeight(void)
{
    return DISPLAY_IS_ORIENTATION_VERTICAL ? DISPLAY_DIMENSION_MACH : DISPLAY_DIMENSION_LESS;
}
