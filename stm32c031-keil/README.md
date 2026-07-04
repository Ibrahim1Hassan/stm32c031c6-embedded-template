# stm32c031c6-embedded-template

Template project for future STM32C031C6 low-level driver development.

This project is intended to be used for future driver development. It contains simple initialization of the STM32C031C6 board peripherals like button, SysTick, UART printf in-out, and a simple event-based state machine.

## Folder Naming Style

The repository is organized with a simple layered naming style that matches the project name and keeps board code separate from reusable drivers:

- `src/app/` for application code and the event-driven state machine.
- `src/board/` for board-specific support code for the NUCLEO-C031C6.
- `src/drivers/` for reusable low-level drivers such as UART, button, and timebase/SysTick.
- `include/app/` for application-level headers.
- `include/board/` for board-level headers.
- `include/drivers/` for driver headers.

## Suggested Naming Rules

- Use the board name in board-specific modules.
- Use generic names for reusable drivers.
- Keep public headers under `include/` and source files under `src/`.
- Keep future sensor or module drivers in `src/drivers/` unless they are board-specific.

## Goal

Use this repository as a clean starting point for adding one driver at a time, while keeping event handling, UART tracing, and board support easy to follow.