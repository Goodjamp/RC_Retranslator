#ifndef __BSP_H__
#define __BSP_H__

/***********************************************************/
/********************        SPI        ********************/
/***********************************************************/
#define SPI_DISPLAY_ILI9341            2

/***********************************************************/
/********************        DMA        ********************/
/***********************************************************/

// ETH SPI (ILI9341)
#define DMA_DISPLAY_ILI9341           2
#define DMA_DISPLAY_ILI9341_CH        5

/*
 *     DMA1
 *   +-----------+---------------------+---------------------+------------------+---------------------+--------------------+---------------------+
 *   | Channel # |         1           |          2          |       3          |         4           |       5            |          6          |
 *   +-----------+---------------------+---------------------+------------------+---------------------+--------------------+---------------------+
 *   | Req       |     SPI_TX          |                     |                  |                     |                    |                     |
 *   +-----------+---------------------+---------------------+------------------+---------------------+--------------------+---------------------+
 *
 *     DMA2
 *   +-----------+---------------------+---------------------+------------------+---------------------+--------------------+---------------------+
 *   | Channel # |         1           |          2          |       3          |         4           |       5            |          6          |
 *   +-----------+---------------------+---------------------+------------------+---------------------+--------------------+---------------------+
 *   | Req       |                     |                     |                  |                     |                    |                     |
 *   +-----------+---------------------+---------------------+------------------+---------------------+--------------------+---------------------+
 * 
 */



/***********************************************************/
/********************       GPIO        ********************/
/***********************************************************/
// Display ILI9341
#define GPIO_DISP_ILI9341_SPI_SCK_PORT        B
#define GPIO_DISP_ILI9341_SPI_SCK_PIN         13
#define GPIO_DISP_ILI9341_SPI_SCK_ALT_FUN     5
#define GPIO_DISP_ILI9341_SPI_MOSI_PORT       B
#define GPIO_DISP_ILI9341_SPI_MOSI_PIN        15
#define GPIO_DISP_ILI9341_SPI_MOSI_ALT_FUN    5
#define GPIO_DISP_ILI9341_SPI_CS_PORT         B
#define GPIO_DISP_ILI9341_SPI_CS_PIN          12
#define GPIO_DISP_ILI9341_RESET_PORT          B
#define GPIO_DISP_ILI9341_RESET_PIN           1
#define GPIO_DISP_ILI9341_CXD_PORT            B
#define GPIO_DISP_ILI9341_CXD_PIN             2
#define GPIO_DISP_ILI9341_LED_PORT            B
#define GPIO_DISP_ILI9341_LED_PIN             0

// DEBUG
/*
#define DEBUG_1_GPIO_PORT                     GPIOE
#define DEBUG_1_GPIO_PIN                      LL_GPIO_PIN_0
#define DEBUG_2_GPIO_PORT                     GPIOE
#define DEBUG_2_GPIO_PIN                      LL_GPIO_PIN_1
#define DEBUG_3_GPIO_PORT                     GPIOB
#define DEBUG_3_GPIO_PIN                      LL_GPIO_PIN_8
#define DEBUG_4_GPIO_PORT                     GPIOB
#define DEBUG_4_GPIO_PIN                      LL_GPIO_PIN_9
#define DEBUG_5_GPIO_PORT                     GPIOE
#define DEBUG_5_GPIO_PIN                      LL_GPIO_PIN_2
#define DEBUG_6_GPIO_PORT                     GPIOE
#define DEBUG_6_GPIO_PIN                      LL_GPIO_PIN_3
*/
#endif // __BSP_H__
