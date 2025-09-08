#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <stdint.h>

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
typedef uint16_t MiddlewareDisplayColor565;

DisplayStatus displayInit(DisplayCb cb);
DisplayStatus displayPutPixel888(displayColor888 *buff, uint32_t pixelCnt);
DisplayStatus displayPutPixel565(MiddlewareDisplayColor565 *buff, uint32_t pixelCnt);
DisplayStatus displaySetArea(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
DisplayStatus displaySetOrientation(DisplayOrientation orientation);
uint32_t displayGetWidth(void);
uint32_t displayGetHeight(void);

#endif
