#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdint.h>

/*
 * The original ILI9341 LCD controller has strong documentation that describes the default display configuration and the default orientation.
 * According to the datasheet, the default orientation is vertical (portrait): the short side (240 px) is the X-axis, and the long side (320 px) is the Y-axis.
 * The display orientation can be changed by writing to the MADCTL register (Memory Access Control), specifically by setting bit MV.
 * When MV = 1:
 * - Register 0x2A (COLUMN address) becomes PAGE address
 * - Register 0x2B (PAGE address) becomes COLUMN address
 * (i.e., X and Y axes are swapped)
 *
 * However, many ILI9341 clones exist. Nobody guarantees that the default register values or the power-on orientation are identical
 * to the original Ilitek ILI9341.
 * The test, described below do not covered all possible display samples based on clone of Ilitek ILI9341. If display behaviour different of target
 * user need tune tests and library.
 *
 * Test description: invoke the function
 * displayDefaultOrientationTest(false, false, false)
 *
 * The possible results
 *
 * +-----------+
 * | -         |
 * | --        |
 * |           |
 * |           |
 * |           |
 * +-----------+
 * Default state configuration: displaySetDefaultState((DisplayDefaultState){.isVertical = false, .isBgr = false});
 *
 *
 * +------------------+
 * | -                |
 * | --               |
 * |                  |
 * +------------------+
 * Default state configuration: displaySetDefaultState((DisplayDefaultState){.isVertical = true, .isBgr = false});
 */

typedef enum {
    DISPLAY_STATUS_OK,
    DISPLAY_STATUS_ERROR_CB,
    DISPLAY_STATUS_ERROR_NOT_INIT,
    DISPLAY_STATUS_ERROR_TX_NULL,
    DISPLAY_STATUS_ERROR_X_OUT_OF_RANGE,
    DISPLAY_STATUS_ERROR_Y_OUT_OF_RANGE,
    DISPLAY_STATUS_ERROR_WIDTH_OUT_OF_RANGE,
    DISPLAY_STATUS_ERROR_HEIGHT_OUT_OF_RANGE,
    DISPLAY_STATUS_ERROR_ORIENTATION_OUT_OF_RANGE,
} DisplayStatus;

typedef enum {
    DISPLAY_ORIENTATION_VERTICAL,
    DISPLAY_ORIENTATION_HORIZONTAL,
    DISPLAY_ORIENTATION_CNT,
} DisplayOrientation;

typedef enum {
    DISPLAY_COORD_ORDER_L_TO_H,
    DISPLAY_COORD_ORDER_H_TO_L,
    DISPLAY_COORD_ORDER_CNT,
} DisplayCoordOrder;

typedef enum {
    DISPLAY_COLOR_ORDER_RGB,
    DISPLAY_COLOR_ORDER_BGR,
    DISPLAY_COLOR_ORDER_CNT,
} DisplayColorOrder;

typedef struct {
    void (*txU8)(uint8_t *data, uint32_t size);
    void (*txU16)(uint16_t *data, uint32_t size);
    void (*spiCs)(uint8_t state);
    void (*cxd)(uint8_t state);
    void (*reset)(uint8_t state);
	  void (*delayMs)(uint32_t delay);
} DisplayCb;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} displayColor888;

/*
 * Color layout:
 *
 * +-------+----+----+----+----+----+----+---+---+---+---+---+---+---+---+---+---+
 * | Bit   | 15 | 14 | 13 | 12 | 11 | 10 | 9 | 8 | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
 * +-------+----+----+----+----+----+----+---+---+---+---+---+---+---+---+---+---+
 * | Color |    Red            |          Green          |       Blue             |
 * +-------+-------------------+-------------------------+------------------------+
 */
typedef struct {
    bool isVertical;
    bool isBgr;
} DisplayDefaultState;

typedef struct {
    DisplayOrientation orientation;
    bool xOrder; // X-axis coordinate order (left→right or right→left).
                 // Due to many possible wiring/orientation variants in ILI9341 modules and clones,
                 // the user must configure this flag according to their display sample.
    bool yOrder; // See xOrder
    DisplayColorOrder colorOrder;
} DisplayConfig;

typedef uint16_t MiddlewareDisplayColor565;
void displayDefaultStatenTest(bool orientation, bool orderX, bool orderY);
void displaySetDefaultState(DisplayDefaultState defaultState);
DisplayStatus displayInit(DisplayCb cb);
DisplayStatus displayPutPixel888(displayColor888 *buff, uint32_t pixelCnt);
DisplayStatus displayPutPixel565(MiddlewareDisplayColor565 *buff, uint32_t pixelCnt);
DisplayStatus displaySetArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
DisplayStatus displaySetConfig(DisplayConfig config);
uint32_t displayGetWidth(void);
uint32_t displayGetHeight(void);

#endif
