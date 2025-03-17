/*============================================================================================================================================================================
Information

Tech        Version         Description
==============================================================================================================================================================================
SDK: 		v2.1.1
Toolchain:	14.2Rel1
Ninja:		v1.12.1
CMake:		v4.0.0-rc4
============================================================================================================================================================================*/



/*============================================================================================================================================================================
Libary or sub-files

Includes    Libaries
============================================================================================================================================================================*/
#include    <stdio.h>
#include    "pico/stdlib.h"
#include    "hardware/pio.h"
#include    "pico/cyw43_arch.h"
#include    "hardware/uart.h"




/*============================================================================================================================================================================
Define      Variable        Value       Description
============================================================================================================================================================================*/



/*============================================================================================================================================================================
Biến cơ bản

Type        Variable        Value       Description
============================================================================================================================================================================*/
// Giả dữ liệu truyền vào
double      power           =50;        // Công xuất động cơ (%) | 0-100
bool        direct          =1;         // Hướng đi: 1 - Tiến | 0 - Lùi
bool        isRight         =1;         // Hướng quay: 1 - Phải | 0 - Trái
uint8_t     STU             =0;         // Trạng thái đặc biệt: 0 - Nomal | 1 - Boot | 2 - Parking | 3 - unParking.

// Giả dữ liệu truyền ra



/*============================================================================================================================================================================
Biến đa luồng

Volatile    Type        Variable        Value       Description
============================================================================================================================================================================*/
// Dữ liệu truyền ra Real-time
volatile    int8_t      in1_e1          =0;        // Động cơ 1 cổng 1
volatile    int8_t      in2_e1          =0;        // Động cơ 1 cổng 2
volatile    int8_t      in1_e2          =0;        // Động cơ 2 cổng 1
volatile    int8_t      in2_e2          =0;        // Động cơ 2 cổng 2
volatile    int8_t      in1_e3          =0;        // Động cơ 3 cổng 1
volatile    int8_t      in2_e3          =0;        // Động cơ 3 cổng 2
volatile    int8_t      in1_e4          =0;        // Động cơ 4 cổng 1
volatile    int8_t      in2_e4          =0;        // Động cơ 4 cổng 2



/*============================================================================================================================================================================
Main Function
============================================================================================================================================================================*/
int main()
{
    
    
}



/*============================================================================================================================================================================
Sub-Functions
============================================================================================================================================================================*/
// Check COM
void check_COM()
{
    if (uart_is_readable(uart0))
    {
        uint8_t data = uart_getc(uart0);
        switch (data)
        {
            case 0x01:
                STU = 1;
                break;
            case 0x02:
                STU = 2;
                break;
            case 0x03:
                STU = 3;
                break;
            default:
                STU = 0;
                break;
        }
    }
}


// Đơn xung
void wave (double power, uint8_t pin) // có thể cho thêm biến truyền vào
{
    // @Nghia: Hàm B (hàm này tạo xung đơn, 1 bước sóng) sẽ tạo ra một xung on-off của một bước sóng với đầu ra là một chân pin x được "truyền" vào sau, số độ dài bước sóng trong một s bằng power * 10e4 (us)
    // ví dụ: in: 50% --> 500ms/1000 <=> 500000us, chân PIN 1 --> out: pin = 1 --> sleep(hoặc busy_wait_ cho chính xác) 10ms (tần số là 50HZ <=> 500/50 = 10 - phần này sẽ có số 50 trong code từ đó biến đổi ra) ---> sau 10ms output = 0.
    // in: power - công suất động cơ (0-100) | chân pin cần tạo sóng.
    // out: xung on-off của một bước sóng với đầu ra là một chân pin x.

    // Define constants for frequency and period
    const uint32_t frequency = 50;          // Frequency in Hz
    const uint32_t period_us = 1000000 / frequency;  // Period in microseconds (20ms at 50Hz)
    // Clamp power to valid range: 0% to 100%
    if (power < 0) power = 0;
    if (power > 100) power = 100;

    // Calculate HIGH and LOW times based on power percentage
    uint32_t high_time_us = (uint32_t)((power / 100.0) * period_us);  // HIGH duration in microseconds
    uint32_t low_time_us = period_us - high_time_us;                  // LOW duration in microseconds

    // Generate the pulse: HIGH followed by LOW
    //sleep
    gpio_put(pin, 1);          // Set pin HIGH
    sleep_us(high_time_us);    // Sleep for HIGH duration
    gpio_put(pin, 0);          // Set pin LOW
    sleep_us(low_time_us);     // Sleep for LOW duration
    //busy_wait  
    gpio_put(pin, 1);          // Set pin HIGH
    busy_wait_us_32(high_time_us);  // Wait for HIGH duration
    gpio_put(pin, 0);          // Set pin LOW
    busy_wait_us_32(low_time_us);   // Wait for LOW duration
}

void the_Waves ()
{
    
}



// Đơn động cơ
// Bên phải
void single_1_e1(double power, bool direct)
{
    in1_e1 = direct ? 0 : power;
    in2_e1 = direct ? power : 0;
}

void single_1_e2(double power, bool direct)
{
    in1_e2 = direct ? 0 : power;
    in2_e2 = direct ? power : 0;
}

// Bên trái
void single_1_e3(double power, bool direct)
{
    in1_e3 = direct ? power : 0;
    in2_e3 = direct ? 0 : power;
}

void single_1_e4(double power, bool direct)
{
    in1_e4 = direct ? power : 0;
    in2_e4 = direct ? 0 : power;
}


// Động bộ dọc 2 động cơ
void sync_2_Vertical_R(double power, bool direct)
{
    single_1_e1(power, direct);
    single_1_e2(power, direct);
}

void sync_2_Vertical_L(double power, bool direct)
{
    single_1_e3(power, direct);
    single_1_e4(power, direct);
}


// Động bộ ngang 2 động cơ
void sync_2_Horizontal_R()
{
}

void sync_2_Horizontal_L()
{
}


// Nghịch đồng bộ 2 - 2 động cơ (Xoay)
void circular(double power, bool isRight)
{
    sync_2_Vertical_R(power, isRight ? 1 : 0);
    sync_2_Vertical_L(power, isRight ? 0 : 1);
}

// Đồng bộ 4
void sync_4(double power, bool direct)
{
    sync_2_Vertical_R(power, direct);
    sync_2_Vertical_L(power, direct);
}


// Giao thức đặc biệt không qua các hàm xung --------------------------------------------------------------------------------------------------------------------------------
void boot()
{

}

void parking()
{

}

void unParking()
{

}

void pause()
{

}




