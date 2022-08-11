#ifndef _BOARD_HARDWARE_H_
#define _BOARD_HARDWARE_H_

#define hardware_v1

#ifdef hardware_v1

#define PORT_LED_ONBOARD GPIOA
#define PIN_LED_ONBOARD GPIO_PIN_1

#define PORT_BTN_ONBOARD GPIOA
#define PIN_BTN_ONBOARD GPIO_PIN_8

// #define PIN_BLUETOOTH_UART_TXO PA2
// #define PIN_BLUETOOTH_UART_RXI PA3

// #define PIN_BIG_BANGING_I2C_SDA PB6
// #define PIN_BIG_BANGING_I2C_SCL PB4

// // nRF2401
// #define PIN_NRF24L01_CE PB0
// #define PIN_NRF24L01_CNS PB2
// #define PIN_NRF24L01_SCK PA5
// #define PIN_NRF24L01_MISO PA6
// #define PIN_NRF24L01_MOSI PA7

// // ESP8266-01
// #define PIN_ESP8266_CE
// #define PIN_ESP8266_RST
// #define PIN_ESP8266_GPIO0
// #define PIN_ESP8266_GPIO2
// #define PIN_ESP8266_RXI PA2
// #define PIN_ESP8266_TXO PA3

// // SDcard
// #define PIN_SDCARD_NC1       // PIN1
// #define PIN_SDCARD_CS PA4    // PIN2
// #define PIN_SDCARD_DI PA7    // PIN3
// #define PIN_SDCARD_VDD       // PIN4
// #define PIN_SDCARD_SCLK PA5  // PIN5
// #define PIN_SDCARD_VSS       // PIN6
// #define PIN_SDCARD_DO PA6    // PIN7
// #define PIN_SDCARD_NC2       // PIN8

#endif

#endif