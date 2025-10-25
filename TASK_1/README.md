
# 🔌 STM32F103 FreeRTOS — Xử lý ngắt ngoài (EXTI) với Semaphore

## 🧩 Giới thiệu
Dự án này minh họa cách **xử lý ngắt ngoài (External Interrupt - EXTI)** kết hợp với **FreeRTOS Semaphore** để đồng bộ giữa **ngắt và task** trên vi điều khiển **STM32F103C8T6**.

Cụ thể:
- **Task 1**: Nhấp nháy LED ở chân `PC13` mỗi 500ms.
- **Task 2**: Chờ ngắt từ nút nhấn `PA0`. Khi nút được nhấn (ngắt xảy ra), task sẽ bật LED ở `PA2` trong 1 giây.

---

## ⚙️ Cấu hình phần cứng
| Thành phần | Chân STM32 | Mô tả |
|-------------|-------------|-------|
| LED 1       | PC13        | Nhấp nháy liên tục (Task Blink) |
| LED 2       | PA2         | Bật khi có ngắt từ nút nhấn |
| Nút nhấn    | PA0 (EXTI0) | Kéo lên nội (Input Pull-Up), kích hoạt mức thấp (Falling Edge) |

> 💡 Gợi ý: Kết nối nút nhấn giữa **PA0** và **GND**, có thể thêm tụ 100nF để chống dội phần cứng.

---

## 🧠 Chức năng chính
- **EXTI0_IRQHandler()**: Xử lý ngắt khi nhấn nút, gửi tín hiệu semaphore đến task.
- **Task Blink**: Chớp LED PC13 định kỳ 500ms.
- **Task Handle**: Chờ semaphore từ ISR, sau đó bật LED PA2 trong 1 giây.

---

## 🧱 Sơ đồ hoạt động
```text
[Button PA0 Pressed]
        │
        ▼
   [EXTI0_IRQHandler]
        │
   xSemaphoreGiveFromISR()
        │
        ▼
   [Task Handle] ---> Bật LED PA2 1s
````

---

## 🧩 Cấu trúc file

```
📁 STM32_FreeRTOS_EXTI_Semaphore
│
├── main.c               // Code chính
├── FreeRTOSConfig.h     // Cấu hình RTOS
├── README.md            // Mô tả dự án
└── (các file HAL/StdPeriph khác)
```

---

## 🔧 Công cụ sử dụng

* **MCU**: STM32F103C8T6
* **IDE**: Keil uVision5 / STM32CubeIDE
* **RTOS**: FreeRTOS v10.x
* **Thư viện**: STM32F10x Standard Peripheral Library

---

## 🚀 Cách hoạt động

1. Khi hệ thống khởi động:

   * LED PC13 bắt đầu nhấp nháy mỗi 500ms.
   * Task `handle` chờ semaphore.
2. Khi nhấn nút tại PA0:

   * ISR (`EXTI0_IRQHandler`) được kích hoạt.
   * ISR “give” semaphore cho Task `handle`.
   * Task `handle` nhận semaphore → bật LED PA2 trong 1s.

---

## 🧩 Cấu hình ưu tiên

* **NVIC_PriorityGroup_4**
* **Ngắt EXTI0**: Ưu tiên thấp hơn Task RTOS
* **Task Blink**: Ưu tiên = 1
* **Task Handle**: Ưu tiên = 2 (cao hơn)



```text
PA0 ----> Nút nhấn ----> GND
PA2 ----> LED (qua điện trở)
PC13 ---> LED Onboard
```

---

