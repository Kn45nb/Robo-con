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
#include    "pico/multicore.h"
#include    "hardware/pio.h"
#include    "hardware/uart.h"




/*============================================================================================================================================================================
Define      Variable        Value       Description
============================================================================================================================================================================*/
// cổng UART0
#define     UART_ID         uart0       // Chọn UART0
#define     TX_PIN          0           // GP_0 (TX)
#define     RX_PIN          1           // GP_1 (RX)
#define     BAUD_RATE       115200      // Tốc độ Baud (tùy chỉnh theo module)
#define     BUFFER_SIZE     8           // Kích thước bộ đệm (tùy chỉnh theo ứng dụng)

// Độ chính xác luồng Engine
#define     UNIT            0           // 0 - ms | 1 - us
#define     ACCURACY        0           // 0 - busy_wait_ | 1 - sleep_
#define     FREQUENCIES     50          // Tần số giao động (Hz)

// Cổng GPIO
#define     GPIO_0          12          // GP_12
#define     GPIO_1          13          // GP_13
#define     GPIO_2          14          // GP_14
#define     GPIO_3          15          // GP_15




/*============================================================================================================================================================================
Biến cơ bản

Type        Variable            Value       Description
============================================================================================================================================================================*/
// Bộ đệm dữ liệu truyền vào
uint8_t     rx_temp[BUFFER_SIZE];           // Bộ đệm truyền vào (@Kn45nb chinhr sửa kích thước theo tùy chọn pack)



/*============================================================================================================================================================================
Biến đa luồng

Volatile    Type        Variable        Value       Description
============================================================================================================================================================================*/
// Trạng thái
volatile    uint8_t     STU             =0;        // Trạng thái đặc biệt: 0 - Null | 1 - Đi | 2 - Rẽ (4 bánh) | 3 - Boot | 4 - Parking | 5 - UnParking | 6 - Pause | 7 - Rẽ (2 bánh).
volatile    uint8_t     power           =50;       // Công xuất động cơ (%) | 0-100
volatile    bool        direct          =1;        // Hướng đi: 1 - Tiến | 0 - Lùi
volatile    bool        isRight         =1;        // Hướng quay: 1 - Phải | 0 - Trái


// Công xuất động cơ truyền ra Real-time
volatile    int8_t      in1_e1          =0;        // Trạng thái động cơ 1 cổng 1
volatile    int8_t      in2_e1          =0;        // Trạng thái động cơ 1 cổng 2
volatile    int8_t      in1_e2          =0;        // Trạng thái động cơ 2 cổng 1
volatile    int8_t      in2_e2          =0;        // Trạng thái động cơ 2 cổng 2
volatile    int8_t      in1_e3          =0;        // Trạng thái động cơ 3 cổng 1
volatile    int8_t      in2_e3          =0;        // Trạng thái động cơ 3 cổng 2
volatile    int8_t      in1_e4          =0;        // Trạng thái động cơ 4 cổng 1
volatile    int8_t      in2_e4          =0;        // Trạng thái động cơ 4 cổng 2



/*============================================================================================================================================================================
Sub-Functions
============================================================================================================================================================================*/
// Config GPIO
void config_gpio()
{
    gpio_init(GPIO_0);
    gpio_set_dir(GPIO_0, GPIO_OUT);

    gpio_init(GPIO_1);
    gpio_set_dir(GPIO_1, GPIO_OUT);

    gpio_init(GPIO_2);
    gpio_set_dir(GPIO_2, GPIO_OUT);

    gpio_init(GPIO_3);
    gpio_set_dir(GPIO_3, GPIO_OUT);
}


// Hàm tạo xung
void wave(uint8_t power)
{
    // Không xài gpio_put_masked() do độ trễ cớ thể xảy ra hoặc do lười. @Kn45nb
    gpio_put(GPIO_0, !!in1_e1);
    gpio_put(GPIO_1, !!in2_e1);
    gpio_put(GPIO_2, !!in1_e2);
    gpio_put(GPIO_3, !!in2_e2);

    UNIT ?
    (ACCURACY ? sleep_us(power * 10000 / FREQUENCIES) : busy_wait_us(power * 10000 / FREQUENCIES))
    :
    (ACCURACY ? sleep_ms(power * 10 / FREQUENCIES) :    busy_wait_ms(power * 10 / FREQUENCIES));

    gpio_put(GPIO_0, 0);
    gpio_put(GPIO_1, 0);
    gpio_put(GPIO_2, 0);
    gpio_put(GPIO_3, 0);
}

// Động bộ dọc 2 động cơ
void sync_2_Vertical_R(uint8_t power, bool direct)
{
    in1_e1 = direct ? 0 : power;
    in2_e1 = direct ? power : 0;
}

void sync_2_Vertical_L(uint8_t power, bool direct)
{
    in1_e1 = direct ? power : 0;
    in2_e1 = direct ? 0 : power;
}


// Nghịch đồng bộ 2 - 2 động cơ (Xoay)
void circular(uint8_t power, bool isRight)
{
    sync_2_Vertical_R(power, isRight);
    sync_2_Vertical_L(power, !isRight);
}

// Đồng bộ 4
void sync_4(uint8_t power, bool direct)
{
    sync_2_Vertical_R(power, direct);
    sync_2_Vertical_L(power, direct);
}


// Giao thức đặc biệt không qua các hàm xung
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


// connect & Check COM
void config_COM()
{
    uart_init(UART_ID, BAUD_RATE);
    gpio_set_function(TX_PIN, GPIO_FUNC_UART);
    gpio_set_function(RX_PIN, GPIO_FUNC_UART);
}

void get_COM(uint8_t* rx_temp, size_t length)
{
    size_t i = 0;
    while (i < length)
    {
        if (uart_is_readable(UART_ID))
        {
            rx_temp[i++] = uart_getc(UART_ID);
        }
    }
}

void check_COM()
{
    while (true)
    {
        get_COM(rx_temp, BUFFER_SIZE);

        STU = rx_temp[0] & 0x0F;
        direct = rx_temp[1] & 0x01;
        isRight = rx_temp[2] & 0x01;
        power = rx_temp[3];
    }
}


void check_Engine()
{
    while (true)
    {
        switch (STU)
        {
        case 0:
            break;
        case 1:
            sync_4(power, direct);
            wave(power);
            UNIT ?
            (ACCURACY ? sleep_us((100 - power) * 10000 / FREQUENCIES) : busy_wait_us((100 - power) * 10000 / FREQUENCIES))
            :
            (ACCURACY ? sleep_ms((100 - power) * 10 / FREQUENCIES) :    busy_wait_ms((100 - power) * 10 / FREQUENCIES));
            break;
        case 2:
            circular(power, isRight);
            wave(power);
            UNIT ?
            (ACCURACY ? sleep_us((100 - power) * 10000 / FREQUENCIES) : busy_wait_us((100 - power) * 10000 / FREQUENCIES))
            :
            (ACCURACY ? sleep_ms((100 - power) * 10 / FREQUENCIES) :    busy_wait_ms((100 - power) * 10 / FREQUENCIES));
            break;
        case 3:
            boot();
            // @Kn45nb hàm chưa được dev
            break;
        case 4:
            parking();
            // @Kn45nb hàm chưa được dev
            break;
        case 5:
            unParking();
            // @Kn45nb hàm chưa được dev
            break;
        case 6:
            pause();
            // @Kn45nb hàm chưa được dev
            break;
        case 7:
            // @Kn45nb Dev sau hiện tại chưa cần thiết.
            break;
        default:
            break;
        }
    }   
}



/*============================================================================================================================================================================
Main Function
============================================================================================================================================================================*/
int main()
{
    stdio_init_all();

    config_gpio();

    // config_COM();

    // multicore_launch_core1(check_Engine);

    // check_COM();

    // @Kn45nb
    // Testing ---------------------------
    while (true)
    {
        sync_4(50, 1);
        for (uint16_t i = 0; i < 500; i++)
        {
            wave(50);
        }

        circular(50, 1);
        for (uint8_t i = 0; i < 20; i++)
        {
            wave(50);
        }
    }
    // -----------------------------------
}