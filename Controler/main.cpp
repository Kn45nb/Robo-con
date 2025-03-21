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
#define     GPIO_0          2           // GP_2
#define     GPIO_1          3           // GP_3
#define     GPIO_2          6           // GP_6
#define     GPIO_3          7           // GP_7
#define     GPIO_4          8           // GP_8
#define     GPIO_5          9           // GP_9
#define     GPIO_6          10          // GP_10
#define     GPIO_7          11          // GP_11



/*============================================================================================================================================================================
Biến cơ bản

Type        Variable            Value       Description
============================================================================================================================================================================*/
// Giả dữ liệu truyền vào
uint8_t     rx_temp[BUFFER_SIZE];           // Bộ đệm truyền vào (@Kn45nb)

// Giả dữ liệu truyền ra (@Kn45nb)
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
// Hàm tạo xung
void wave(uint8_t power)
{
    // @Kn45nb, xài: add_repeating_timer_ms() và cancel_repeating_timer()
    in1_e1 ? out1_e1 = 1 : out1_e1 = 0;
    in2_e1 ? out2_e1 = 1 : out2_e1 = 0;
    in1_e2 ? out1_e2 = 1 : out1_e2 = 0;
    in2_e2 ? out2_e2 = 1 : out2_e2 = 0;
    in1_e3 ? out1_e3 = 1 : out1_e3 = 0;
    in2_e3 ? out2_e3 = 1 : out2_e3 = 0;
    in1_e4 ? out1_e4 = 1 : out1_e4 = 0;
    in2_e4 ? out2_e4 = 1 : out2_e4 = 0;

    UNIT ?
    (ACCURACY ? sleep_us(power * 10000 / FREQUENCIES) : busy_wait_us(power * 10000 / FREQUENCIES))
    :
    (ACCURACY ? sleep_ms(power * 10 / FREQUENCIES) :    busy_wait_ms(power * 10 / FREQUENCIES));

    // @Kn45nb
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
void single_1_e1(uint8_t power, bool direct)
{
    in1_e1 = direct ? 0 : power;
    in2_e1 = direct ? power : 0;
}

void single_1_e2(uint8_t power, bool direct)
{
    in1_e2 = direct ? 0 : power;
    in2_e2 = direct ? power : 0;
}

// Bên trái
void single_1_e3(uint8_t power, bool direct)
{
    in1_e3 = direct ? power : 0;
    in2_e3 = direct ? 0 : power;
}

void single_1_e4(uint8_t power, bool direct)
{
    in1_e4 = direct ? power : 0;
    in2_e4 = direct ? 0 : power;
}


// Động bộ dọc 2 động cơ
void sync_2_Vertical_R(uint8_t power, bool direct)
{
    single_1_e1(power, direct);
    single_1_e2(power, direct);
}

void sync_2_Vertical_L(uint8_t power, bool direct)
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

// Config and check Engine
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

    gpio_init(GPIO_4);
    gpio_set_dir(GPIO_4, GPIO_OUT);

    gpio_init(GPIO_5);
    gpio_set_dir(GPIO_5, GPIO_OUT);

    gpio_init(GPIO_6);
    gpio_set_dir(GPIO_6, GPIO_OUT);

    gpio_init(GPIO_7);
    gpio_set_dir(GPIO_7, GPIO_OUT);
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
            wave(power);    // @Kn45nb
            break;
        case 2:
            circular(power, isRight);
            wave(power);    // @Kn45nb
            break;
        case 3:
            boot();
            break;
        case 4:
            parking();
            break;
        case 5:
            unParking();
            break;
        case 6:
            pause();
            break;
        case 7:
            // @Kn45nb
            break;
        default:
            break;
        }

        // @Kn45nb
        UNIT ?
        (ACCURACY ? sleep_us((100 - power) * 10000 / FREQUENCIES) : busy_wait_us((100 - power) * 10000 / FREQUENCIES))
        :
        (ACCURACY ? sleep_ms((100 - power) * 10 / FREQUENCIES) :    busy_wait_ms((100 - power) * 10 / FREQUENCIES));
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

    // Testing
    while(true)
    {
        gpio_put(2, 1);
        gpio_put(3, 0);
        sleep_ms(1000);
        gpio_put(2, 0);
        gpio_put(3, 1);
        sleep_ms(1000);
    }
}