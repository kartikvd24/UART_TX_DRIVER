# STM32F4 USART2 TX Bare-Metal UART Example

## Overview

This project demonstrates simple UART transmission using USART2 on an STM32F4 microcontroller with direct register-level programming (bare-metal). It configures GPIOA pin 2 as USART2 TX (alternate function AF7) and continuously sends the character `'N'` via UART at 115200 baud.

No HAL or CubeMX libraries are used — only direct peripheral register access.

---

## Features

- Direct register access for UART communication
- Configures PA2 for USART2 TX functionality
- Sets baud rate to 115200 (configurable)
- Continuously transmits character `'N'`

---

## Hardware Requirements

- STM32F4-series MCU (e.g., STM32F401, STM32F407)
- UART-to-USB converter (e.g., FTDI, CP2102) or virtual COM port (if ur board is not the classic nucleo or development board) 
- Serial terminal software (RealTerm)

### Connections

- STM32 PA2 (USART2_TX) → UART adapter RX
- Common ground between STM32 board and UART adapter

---

## Configuration

- **Clock frequency:**  
  Update `SYS_FREQ` and `APB1_CLK` macros in the code to reflect your MCU’s actual clock setting. These affect baud rate calculation.  
  Example:
#define SYS_FREQ 16000000 // Adjust as needed, e.g., 42000000
#define APB1_CLK SYS_FREQ // Adjust as per APB1 clock frequency


- **Baud rate:**  
Configured via `UART_BAUD_RATE` macro (default 115200).

---

## Code Summary

- Enables GPIOA clock and UART2 clock
- Configures PA2 pin mode to Alternate Function for UART TX
- Sets USART2 baud rate by calculating BRR register based on APB1 clock
- Enables TX mode and UART peripheral
- Continuously writes `'N'` character in an infinite loop

---

## Building and Flashing

- Use your preferred ARM toolchain (ARM GCC, Keil, IAR, STM32CubeIDE, etc.)
- Compile and flash the code to your STM32F4 MCU
- Make sure to set linker scripts and startup code according to your environment

---

## Usage

1. Connect UART adapter RX to MCU PA2, and common ground.
2. Open a serial terminal at the configured baud rate (`115200` by default), 8 data bits, no parity, 1 stop bit, no flow control.
3. Power up your MCU and run the program; you should see a continuous stream of `N` characters.

---

## Troubleshooting

| Symptom                  | Possible Cause                                      | Solution                                  |
|--------------------------|---------------------------------------------------|-------------------------------------------|
| Garbage/random symbols    | Baud rate mismatch due to wrong clock configuration| Verify and set correct `SYS_FREQ` and `APB1_CLK` values in code |
| No output visible         | Incorrect wiring or MCU not running code           | Check wiring and ensure code is flashed & running                |
| No output & terminal stuck| Terminal settings incorrect                        | Set terminal to 115200 baud, 8N1, no flow control               |

---

## Example Code Snippet

#define GPIOAEN (1U<<0)
#define UART_EN (1U<<17)
#define SYS_FREQ 16000000
#define APB1_CLK SYS_FREQ
#define UART_BAUD_RATE 115200
#define CR1_TE (1U<<3)
#define SR_TXE (1U<<7)

static uint16_t compute_uart_div(uint32_t Perif_clk, uint32_t Baudrate) {
return ((Perif_clk + (Baudrate/2U)) / Baudrate);
}

void UART2_TX_INIT(void) {
RCC->AHB1ENR |= GPIOAEN; // Enable GPIOA clock
GPIOA->MODER |= (1U << 5); // PA2 alternate function mode
GPIOA->MODER &= ~(1U << 4);
GPIOA->AFR |= (1U << 8) | (1U << 9) | (1U << 10); // AF7 for PA2
GPIOA->AFR &= ~(1U << 11);
RCC->APB1ENR |= UART_EN;          // Enable USART2 clock
USART2->BRR = compute_uart_div(APB1_CLK, UART_BAUD_RATE);
USART2->CR1 = CR1_TE;             // Enable transmitter
USART2->CR1 |= (1U << 13);        // Enable USART
}

void uart2_write(int ch) {
while (!(USART2->SR & SR_TXE)) {} // Wait for TX buffer empty
USART2->DR = ch & 0xff; // Send data
}

int main(void) {
UART2_TX_INIT();
while (1) {
uart2_write('N');
}
}

Feel free to customize this README as needed for your documentation! If you want, I can also provide a sample linker script or build instructions tailored to your development setup.
