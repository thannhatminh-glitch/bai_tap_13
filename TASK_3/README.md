
# 🧩 STM32 FreeRTOS UART Mutex Example

## 📘 Giới thiệu
Dự án minh họa cách **sử dụng FreeRTOS trên STM32F103C8T6** để thực hiện giao tiếp **UART (USART1)** giữa nhiều **task song song**.  
Hai task (`taskA` và `taskB`) cùng gửi dữ liệu ra cổng UART, nhưng được **bảo vệ bởi Mutex** để tránh trộn lẫn dữ liệu.

---

## ⚙️ Cấu hình phần cứng

| Thành phần | Mô tả |
|-------------|--------|
| Vi điều khiển | STM32F103C8T6 |
| UART sử dụng | USART1 |
| Chân TX | PA9 |
| Chân RX | PA10 |
| Baudrate | 9600 bps |
| Môi trường | FreeRTOS + STM32 Standard Peripheral Library |

> 💡 Nối UART TX (PA9) tới RX của USB-UART Converter, và RX (PA10) tới TX của converter.  
Mở terminal (PuTTY, RealTerm, TeraTerm...) ở **baudrate 9600**, 8N1.

---

## 🧠 Chức năng chính
- **Task A**: gửi chuỗi `"TaskA: ABCDEFGH\r\n"` mỗi 500ms.  
- **Task B**: gửi chuỗi `"TaskB: abcdefgh\r\n"` mỗi 500ms.  
- **UART**: cấu hình ở chế độ 8N1, tốc độ 9600bps.  
- **Mutex**: bảo vệ vùng gửi UART, tránh xung đột khi nhiều task cùng truy cập.

---

## 📄 Luồng hoạt động

1. Hệ thống khởi tạo:
   - Cấu hình clock, GPIO, UART.
   - Tạo Mutex `xUartMutex`.
   - Tạo hai task `taskA` và `taskB`.

2. Trong mỗi task:
   - Lấy Mutex (xSemaphoreTake) trước khi gửi UART.
   - Gửi chuỗi ký tự.
   - Trả Mutex (xSemaphoreGive) sau khi gửi xong.
   - Gọi `vTaskDelay()` để nhường CPU.

3. FreeRTOS Scheduler luân phiên chạy các task dựa trên **độ ưu tiên** và **thời gian delay**.

---

## 📤 Kết quả quan sát

Trên UART Terminal (9600 baud), dữ liệu hiển thị xen kẽ rõ ràng:

```

TaskA: ABCDEFGH
TaskB: abcdefgh
TaskA: ABCDEFGH
TaskB: abcdefgh
...

```

Không có hiện tượng chồng chéo hoặc xen ký tự, nhờ **mutex bảo vệ UART**.

---

## 📂 Cấu trúc thư mục

```

├── main.c           # Code chính (FreeRTOS + UART + Mutex)
├── FreeRTOSConfig.h # Cấu hình hệ điều hành FreeRTOS
├── README.md        # Tài liệu mô tả (file này)
└── STM32F10x_StdPeriph_Driver/
├── inc/
└── src/

```

---

## 🔧 Mở rộng đề xuất
- Thêm **Task C** đọc dữ liệu UART RX và phản hồi lại.  
- Dùng **Queue** để gửi chuỗi giữa các task.  
- Hiển thị dữ liệu nhận được lên **LCD hoặc OLED**.  
- Dùng **DMA UART** để giảm tải CPU.  

---

## 🧰 Công cụ biên dịch
- IDE: STM32CubeIDE, Keil µVision hoặc IAR EWARM  
- RTOS: FreeRTOS v10.x  
- Thư viện: STM32F10x Standard Peripheral Library  

---

Mình có thể vẽ và thêm trực tiếp vào README.md dưới dạng hình hoặc mã Markdown.

