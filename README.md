## config
Please define your LED Stripe Type in HSDLeds.hpp.

For Ledtype with DATAPIN only, like Neopixel, comment out
`#define HASCLOCKPIN`

For Ledtype with DATAPIN AND COLORORDER, like WS2811, comment out
`#define HASCLOCKPINANDCOLORORDER`

For Ledtype that has DATAPIN, CLOCKPIN an COLORORDER, like APA102, comment out `#define HASALL`

Even ** don't forget ** to define right choices in `#ifdef` blocks to fit
your setup.

** Example **
APA102 LED Stripe setup:
comment out `#define HASALL`
set DATAPIN, CLOCKPIN an COLORORDER in
`#ifdef HASALL
  #define DATAPIN 3
  #define CLOCKPIN 2
  #define COLORORDER BGR
#endif`