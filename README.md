## config
Please define your LED Stripe Type in HSDLeds.hpp:

For Ledtype with DATAPIN only, like Neopixel comment out
`#define HASCLOCKPIN`

For Ledtype with DATAPIN AND COLORORDER, like WS2811 comment out
`#define HASCLOCKPINANDCOLORORDER`

For Ledtype that has DATAPIN, CLOCKPIN an COLORORDER, like APA102, comment out `#define HASALL`