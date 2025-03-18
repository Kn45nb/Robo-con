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
#include    "pico/multicore.h"



/*============================================================================================================================================================================
Define      Variable        Value       Description
============================================================================================================================================================================*/
#define     UART_ID         uart0       // Chọn UART0
#define     TX_PIN          0           // GPIO 0 (TX)
#define     RX_PIN          1           // GPIO 1 (RX)
#define     BAUD_RATE       115200      // Tốc độ Baud (tùy chỉnh theo module)



/*============================================================================================================================================================================
Biến cơ bản

Type        Variable        Value       Description
============================================================================================================================================================================*/
// Giả dữ liệu truyền vào
double      power           =50;        // Công xuất động cơ (%) | 0-100
bool        direct          =1;         // Hướng đi: 1 - Tiến | 0 - Lùi
bool        isRight         =1;         // Hướng quay: 1 - Phải | 0 - Trái
uint8_t     STU             =0;         // Trạng thái đặc biệt: 0 - Null | 1 - Đi | 2 - Rẽ (4 bánh) | 3 - Boot | 4 - Parking | 5 - UnParking | 6 - Pause | 7 - Rẽ (2 bánh).

// Giả dữ liệu truyền ra
bool        out1_e1         =0;         // Động cơ 1 cổng 1
bool        out2_e1         =0;         // Động cơ 1 cổng 2
bool        out1_e2         =0;         // Động cơ 2 cổng 1
bool        out2_e2         =0;         // Động cơ 2 cổng 2
bool        out1_e3         =0;         // Động cơ 3 cổng 1
bool        out2_e3         =0;         // Động cơ 3 cổng 2
bool        out1_e4         =0;         // Động cơ 4 cổng 1
bool        out2_e4         =0;         // Động cơ 4 cổng 2



/*============================================================================================================================================================================
Biến đa luồng

Volatile    Type        Variable        Value       Description
============================================================================================================================================================================*/
// Dữ liệu truyền ra Real-time
volatile    int8_t      in1_e1          =0;        // Trạng thái động cơ 1 cổng 1
volatile    int8_t      in2_e1          =0;        // Trạng thái động cơ 1 cổng 2
volatile    int8_t      in1_e2          =0;        // Trạng thái động cơ 2 cổng 1
volatile    int8_t      in2_e2          =0;        // Trạng thái động cơ 2 cổng 2
volatile    int8_t      in1_e3          =0;        // Trạng thái động cơ 3 cổng 1
volatile    int8_t      in2_e3          =0;        // Trạng thái động cơ 3 cổng 2
volatile    int8_t      in1_e4          =0;        // Trạng thái động cơ 4 cổng 1
volatile    int8_t      in2_e4          =0;        // Trạng thái động cơ 4 cổng 2



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
        switch (uart_getc(uart0))
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

void check_Engine()
{
    //@Nghĩa: Viết thêm case dựa vào Description của biến STU. Và Dev phần gọi hàm bên trong các Case đó luôn. Hàm này sẽ được chạy liên tục vậy nên ô thêm lệnh delay (1000 - power) như lần trước.
    switch (STU)
    {
    case 0:
        // không có trạng thái nào được chọn
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 1:
        // Đi (Go) - cho xe chạy thẳng hoac lùi
        sync_4(power, direct);
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 2:
        // Rẽ (4 bánh) rẽ bằng cách sử dụng 4 bánh xe 
        circular(power, isRight);
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 3:
        // Boot - khởi động lại hệ thống
        boot();
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 4:
        // Parking - 
        parking();
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 5:
        // UnParking - 
        unParking();
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 6:
        // Pause - dừng xe
        pause();
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    case 7:
        // Rẽ (2 bánh) 
        circular(power, isRight);
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    default:
        // không có trạng thái nào được chọn
        sleep_ms(1000 - (uint32_t)(power * 10));
        break;
    }
}

// Hàm tạo xung
void wave(double power)
{
    // Tần số giao động
    const uint32_t frequency = 50;

    in1_e1 ? out1_e1 = 1 : out1_e1 = 0;
    in2_e1 ? out2_e1 = 1 : out2_e1 = 0;
    in1_e2 ? out1_e2 = 1 : out1_e2 = 0;
    in2_e2 ? out2_e2 = 1 : out2_e2 = 0;
    in1_e3 ? out1_e3 = 1 : out1_e3 = 0;
    in2_e3 ? out2_e3 = 1 : out2_e3 = 0;
    in1_e4 ? out1_e4 = 1 : out1_e4 = 0;
    in2_e4 ? out2_e4 = 1 : out2_e4 = 0;

    sleep_us((uint32_t)((power * 10000) / frequency));
    // busy_wait_us_32((uint32_t)((power * 10000) / frequency));

    out1_e1 = 0;
    out2_e1 = 0;
    out1_e2 = 0;
    out2_e2 = 0;
    out1_e3 = 0;
    out2_e3 = 0;
    out1_e4 = 0;
    out2_e4 = 0;
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


