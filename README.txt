Project structure


Project
 |
 +--App
 |   |
 |   +---main.c
 |   |
 |   +---main.h
 |   |
 |   +---Version.h
 |
 +--Middleware
 |   |
 |   +---MiddlewareDisplay
 |   |    |
 |   |    +---Display.c
 |   |    |
 |   |    +---MiddlewareDisplay.h
 |   |
 |   |
 |   +---RTOS
 |       |
 |       +---RTOS src
 |
 +--BSP
 |   |
 |   +---BspGpio
 |   |
 |   +---BspSpiDisplayIli9341
 |   |
 |   +---BspDebugServices
 |   |
 |   +---Services
 |   |
 |   +---BSP.h
 |
 +---PortBsp   (Implement bridge between various Project levels, for example:
 |   |          - BspSpiDisplayIli9341 -> MiddlewareDisplay
 |   |          - MiddlewareDisplay -> Gui
 |   |          and so on )
 |   |
 |   +---BspGpio
 |
 +---HAL  (SDK and MCU specific code. The BSP layer is utilize files from HAL to implement BSP abstraction )