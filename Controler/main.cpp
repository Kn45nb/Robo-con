/*============================================================================================================================================================================
Information
==============================================================================================================================================================================
SDK: 		v2.1.1
Toolchain:	14.2Rel1
Ninja:		v1.12.1
CMake:		v4.0.0-rc4
============================================================================================================================================================================*/



/*============================================================================================================================================================================
Includes    Libaries
============================================================================================================================================================================*/
#include    <stdio.h>
#include    "pico/stdlib.h"
#include    "hardware/pio.h"
#include    "pico/cyw43_arch.h"
#include    "hardware/uart.h"



/*============================================================================================================================================================================
Define      Variable    Value   Description
============================================================================================================================================================================*/
#define     RELAY_PIN   16      // Example GPIO pin for relay control. Replace as needed.


/*============================================================================================================================================================================
Sub-Functions
============================================================================================================================================================================*/



// chuyển đối giá trị int8 (-128 đến 127) thành thời gian (trong ms) trong một chu kỳ 1 giây.
// chuyển đổi từ phạm vi -128 đến 127 thành thời gian trễ tính bằng mili giây giữa 0 và ~1000 ms.
uint32_t int8_to_wave_time(int8_t value)
{
    uint8_t norm = static_cast<uint8_t>(value + 128);
    return (norm * 1000UL) / 256;
}


// Hàm phụ để điều khiển relay bằng cách sử dụng giá trị sóng.
// bật và tắt relay trong khoảng thời gian được tính từ giá trị sóng int8_t (dao động từ -128 đến 127)
void controlRelay(int8_t wave_value) 
{
    uint32_t delay_ms = int8_to_wave_time(wave_value);
    gpio_put(RELAY_PIN, 1);  // Turn relay ON.
    sleep_ms(delay_ms);
    gpio_put(RELAY_PIN, 0);  // Turn relay OFF.
    sleep_ms(delay_ms);
}

// để điều khiển động cơ thông qua rơ le.
class MotorController
{
    public:
        MotorController() {
            gpio_init(RELAY_PIN);
            gpio_set_dir(RELAY_PIN, GPIO_OUT);
        }
    
        // Sub-function to control the motor via the relay.
        void controller_motor(uint32_t run_time_ms) {
            gpio_put(RELAY_PIN, 1);  // Activate relay (motor ON)
            sleep_ms(run_time_ms);   // Run motor for the specified duration
            gpio_put(RELAY_PIN, 0);  // Deactivate relay (motor OFF)
        }
    };


// Điều khiển bật/tắt relay để mô phỏng các hành vi quay bánh, phanh và dừng khẩn cấp

    class MotorController {
        public:
            RoboConMotorController() {
                gpio_init(RELAY_PIN);          // Khởi tạo chân GPIO cho relay
                gpio_set_dir(RELAY_PIN, GPIO_OUT); // Đặt chế độ đầu ra cho relay
            }
        
            // Quay đều 4 bánh xe (All Motor)
            void allMotor(uint32_t duration_ms) {
                gpio_put(RELAY_PIN, 1);       // Bật relay (động cơ quay)
                sleep_ms(duration_ms);        // Giữ trạng thái quay trong thời gian xác định
                gpio_put(RELAY_PIN, 0);       // Tắt relay (dừng động cơ)
            }
        
            // Quay hai bánh ngược chiều nhau (Cycle)
            void cycle(uint32_t duration_ms) {
                gpio_put(RELAY_PIN, 1);       // Bật relay (mô phỏng hai bánh quay ngược nhau)
                sleep_ms(duration_ms);        // Giữ quay trong thời gian xác định
                gpio_put(RELAY_PIN, 0);       // Tắt relay
            }
        
            // Khóa cứng bánh xe, mô phỏng phanh tay (Pause)
            void pause() {
                gpio_put(RELAY_PIN, 1);       // Bật relay để khóa bánh
                sleep_ms(100);                // Giữ trạng thái khóa tạm thời
                gpio_put(RELAY_PIN, 0);       // Tắt relay (kết thúc phanh)
            }
        
            // Mở khóa bánh xe, nhả phanh (Free)
            void free() {
                gpio_put(RELAY_PIN, 0);       // Tắt relay để bánh tự do
            }
        
            // Quay một bên bánh xe (Part Cycle)
            void partCycle(uint32_t duration_ms) {
                gpio_put(RELAY_PIN, 1);       // Bật relay (mô phỏng một bên bánh quay)
                sleep_ms(duration_ms);        // Giữ quay trong thời gian xác định
                gpio_put(RELAY_PIN, 0);       // Tắt relay
            }
        
            // Khởi động động cơ ở 100% công suất (Boot)
            void boot() {
                gpio_put(RELAY_PIN, 1);       // Bật relay (full công suất)
                sleep_ms(1000);               // Chạy động cơ trong 1 giây
                gpio_put(RELAY_PIN, 0);       // Tắt relay
            }
        
            // Dừng khẩn cấp bằng cách đảo chiều động cơ (Emergency Stop)
            void emergencyStop() {
                gpio_put(RELAY_PIN, 1);       // Bật relay (giả lập đảo chiều/phanh động cơ)
                sleep_ms(100);                // Giữ trong 100ms để dừng
                gpio_put(RELAY_PIN, 0);       // Tắt relay
            }
        };

/*============================================================================================================================================================================
Main Function
============================================================================================================================================================================*/
int main()
{
    while (true)
    {
        /* code */
    }
    
}