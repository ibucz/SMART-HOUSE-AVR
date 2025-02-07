# Smart Home System Using AVR

This project implements a **Smart Home System** using an **AVR microcontroller**, enabling control over **air conditioning, lighting, security, and wireless communication**. The system integrates various peripherals and sensors to provide a fully automated and interactive home automation solution.

---

## Features

- **Air Conditioning Control:**
  - Uses a **temperature sensor** to monitor and regulate room temperature.
  - Activates cooling based on user-defined thresholds.

- **Lighting System:**
  - **5 LEDs** simulate different light zones.
  - Lights can be controlled manually via a **keypad** or remotely via **Bluetooth**.

- **Security System:**
  - Integrated **keypad** for secure access control.
  - **EEPROM** stores authentication credentials.
  - Unauthorized access triggers an alert.

- **Wireless Communication:**
  - Utilizes a **Bluetooth module** for remote control via a smartphone or external device.

- **User Interface:**
  - **LCD Display** provides real-time system status and user feedback.
  
---

## Tools & Components

- **Microcontroller:** AVR-based
- **Simulation:** Proteus
- **IDE:** Microchip Studio
- **Storage:** EEPROM
- **Display:** LCD Screen
- **Input:** Keypad
- **Sensors:** Temperature Sensor
- **Communication:** Bluetooth Module
- **Indicators:** 5 LEDs

---

## Project Structure

```
Smart-Home-AVR/
│-- src/        # Source code for AVR firmware
│-- inc/        # Header files and API declarations
│-- docs/       # Documentation and implementation details
│-- simulation/ # Proteus simulation files
│-- README.md   # Project description and setup guide
```

---

## How to Use

1. Clone this repository:
   ```sh
   git clone https://github.com/ibucz/Smart-Home-AVR.git
   ```
2. Open the project in **Microchip Studio**.
3. Load the **Proteus simulation** files for testing.
4. Compile and upload the firmware to the **AVR microcontroller**.
5. Interact with the system via **keypad** or **Bluetooth**.

---

## License

This project is licensed under the **MIT License**.

---

## Contributors
- [AHMED ABDULAZIZ](https://github.com/ibucz)

Feel free to contribute to this project by submitting issues or pull requests! 🚀

