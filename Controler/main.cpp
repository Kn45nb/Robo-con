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
Define      Variable        Value       Description
============================================================================================================================================================================*/



/*============================================================================================================================================================================
Type        Variable        Value       Description
============================================================================================================================================================================*/
// Giả dữ liệu truyền vào
float       power           =50;        // Công xuất động cơ (%) | 0-100
bool        direct          =1;         // Hướng đi: 1 - Tiến | 0 - Lùi
bool        isRight         =1;         // Hướng quay: 1 - Phải | 0 - Trái
uint8_t     STU             =0;         // Trạng thái đặc biệt: 0 - Nomal | 1 - Boot | 2 - Parking | 3 - unParking.

// Giả dữ liệu cổng truyền ra
bool        in1_e1          =0;        // Động cơ 1 cổng 1
bool        in2_e1          =0;        // Động cơ 1 cổng 2
bool        in1_e2          =0;        // Động cơ 2 cổng 1
bool        in2_e2          =0;        // Động cơ 2 cổng 2
bool        in1_e3          =0;        // Động cơ 3 cổng 1
bool        in2_e3          =0;        // Động cơ 3 cổng 2
bool        in1_e4          =0;        // Động cơ 4 cổng 1
bool        in2_e4          =0;        // Động cơ 4 cổng 2

// Giả dữ liệu truyền ra



/*============================================================================================================================================================================
Main Function
============================================================================================================================================================================*/
int main()
{
    while (true)
    {
        switch (STU)
        {
        case 1:
            /* code */
            break;
        
        default:
            break;
        }
    }
}



/*============================================================================================================================================================================
Sub-Functions
============================================================================================================================================================================*/
// Chuyển đổi dữ liệu mức công xuất ra xung | 0-100 -> 0-1000
void power_to_time_working(double power, bool direct)
{
    
}

// Đơn động cơ
void single_1_e1(double power, bool direct)
{
    if (direct)
    {
        in1_e1 = 0;
        in2_e1 = power_to_time_working(power);
    }  
    else
    {
        in1_e1 = power_to_time_working(power);
        in2_e1 = 0;
    } 
}

void single_1_e2( float power, bool direct)
{
    if (direct)
    {
        // Backward
        in1_e2 = 0;
        in2_e2 = power_to_time_working(power);
    }
    else
    {
        // Forward
            in1_e2 = power_to_time_working(power);
            in2_e2 = 0;
    }
}

void single_1_e3( float power, bool direct)
    {
        if (direct)

        {
            // Backward
            in1_e3 = 0;
            in2_e3 = power_to_time_working(power);
        }
        else
        {
            // Forward
            in1_e3 = power_to_time_working(power);
            in2_e3 = 0;
        }
    }

void single_1_e4( float power, bool direct)
    {
        if (direct)
        {
            // Backward
            in1_e4 = 0;
            in2_e4 = power_to_time_working(power);
        }
        else
        {
            // Forward
            in1_e4 = power_to_time_working(power);
            in2_e4 = 0;
        }
    }



// Động bộ 4 động cơ
void sync_4()
{
}

// Động bộ dọc 2 động cơ
void sync_2_Vertical_R()
{
}
void sync_2_Vertical_L()
{
}

// Động bộ ngang 2 động cơ
void sync_2_Horizontal_R()
{
}
void sync_2_Horizontal_L()
{
}

// Nghịch đồng bộ 2 - 2 động cơ (Xoay)
void circular()
{
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




