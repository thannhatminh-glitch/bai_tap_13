


# 🧠 STM32 FreeRTOS UART Multitasking Example

## 📘 Giới thiệu
Dự án này minh họa **cách sử dụng FreeRTOS trên STM32F103C8T6** để chạy **hai task song song** gửi dữ liệu qua **UART1**.  
Mỗi task gửi một chuỗi ký tự riêng biệt, thể hiện cơ chế **đa nhiệm (multitasking)** của FreeRTOS — các task cùng chia sẻ tài nguyên UART mà không cần ngắt.

---

## ⚙️ Phần cứng sử dụng
- **Vi điều khiển:** STM32F103C8T6 (“Blue Pill”)
- **Clock:** 72 MHz (SystemInit mặc định)
- **UART1 TX:** PA9  
- **UART1 RX:** PA10  
- **Kết nối UART:**  
  - TX (PA9) → RX của USB-UART (PL2303/CH340/CP2102)
  - GND STM32 nối GND USB-UART
  - Mở terminal: **Baudrate 9600**, **8N1**

---

## 🧩 Chức năng chính

### 🧵 Task A
Gửi chuỗi ký tự hoa:
```

ABCDEFGH

```

### 🧵 Task B
Gửi chuỗi ký tự thường:
```

abcdefgh

```

### 🧠 Kết quả
Hai task cùng gửi xen kẽ ký tự qua UART.  
Khi quan sát trên terminal, bạn sẽ thấy dữ liệu luân phiên như:
```

AaBbCcDdEeFfGgHh
ABCDEFGH
abcdefgh
...

```

→ chứng minh hai task chạy song song và chia sẻ tài nguyên UART1 thông qua FreeRTOS scheduler.

---

## 📜 Cấu trúc mã nguồn

```

├── main.c
├── FreeRTOS/
│   ├── FreeRTOSConfig.h
│   ├── source files ...
├── README.md

````

---

## 🧰 Cấu hình phần mềm

| Thành phần | Mô tả |
|-------------|--------|
| **IDE** | Keil uVision / STM32CubeIDE / PlatformIO |
| **RTOS** | FreeRTOS |
| **Thư viện chuẩn** | STM32F10x Standard Peripheral Library |
| **Tốc độ baud** | 9600 bps |
| **Clock** | 72 MHz |

---





