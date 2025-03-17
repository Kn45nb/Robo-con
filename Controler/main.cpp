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
float       power           50          // Công xuất động cơ | 0-100
bool        direct          1           // Hướng đi: 1 - Forward | 0 - Backward
bool        isRight         1           // Hướng quay: 1 - Right | 0 - Left
uint16_t    STU             50          // Trạng thái đặc biệt: Boot - 1 | Parking - 2 | unParking - 3.

// Giả dữ liệu cổng truyền ra
uint8_t     in1_e1          15          // Động cơ 1 - 1
uint8_t     in2_e2          16          // Động cơ 1 - 2
uint8_t     in1_e3          17          // Động cơ 1 - 3
uint8_t     in2_e4          18          // Động cơ 1 - 4

// Giả dữ liệu truyền ra




/*============================================================================================================================================================================
Sub-Functions
============================================================================================================================================================================*/
// Chuyển đổi dữ liệu mức công xuất ra xung | 0-100 -> 0-1000
public uin16_t power_to_time_working(fload power)
{
    power = power * 10;
    return power;
}

// Đơn động cơ
private void single_1_e1(float power, bool direct)
{

    if (direct == 1) // Nếu hướng quay là tiến
    {
        // Thiết lập tín hiệu để quay tiến
        in1_e1 = power_to_time_working(power); // in1 nhận giá trị tương ứng với công suất
        in2_e1 = 0; // in2 đặt về 0 để đảm bảo chỉ quay 1 chiều
    }
    else // Nếu hướng quay là lùi
    {
        // Thiết lập tín hiệu để quay lùi
        in1_e1 = 0;
        in2_e1 = power_to_time_working(power); // in2 nhận giá trị tương ứng với công suất
    }
}

private void single_1_e2( float power, bool direct)
{
    if (direct  == 1)
    {
    // Forward
        in1_e2 = power_to_time_working(power);
        in2_e2 = 0;
    }
    else
    {
        // Backward
        in1_e2 = 0;
        in2_e2 = power_to_time_working(power);
    }
}

private void single_1_e3( float power, bool direct)
    {
        if (direct == 1)
        {
            // Forward
            in1_e3 = power_to_time_working(power);
            in2_e3 = 0;
        }
        else
        {
            // Backward
            in1_e3 = 0;
            in2_e3 = power_to_time_working(power);
        }
    }
    {
        return 0;
    }

    private void single_1_e4( float power, bool direct)
    {
        if (direct == 1)
        {
            // Forward
            in1_e4 = power_to_time_working(power);
            in2_e4 = 0;
        }
        else
        {
            // Backward
            in1_e4 = 0;
            in2_e4 = power_to_time_working(power);
        }
    }
    {
        return 0;
    }

// Động bộ 4 động cơ
public void sync_4(int power, bool direct)
{
    if (direct)
    {
        // Tiến về phía trước
        single_1_e1(power, direct);
        single_1_e2(power, direct);
        single_1_e3(power, direct);
        single_1_e4(power, direct);
    }
    else
    {
        // Lùi về phía sau
        single_1_e1(power, direct);
        single_1_e2(power, direct);
        single_1_e3(power, direct);
        single_1_e4(power, direct);
    }
}

// Động bộ dọc 2 động cơ bên phải
public void sync_2_Vertical_R(bool direct, int power)
{
    if (direct)
    {
        // Tiến về phía trước
        single_1_e1(power, direct);
        single_1_e2(power, direct);
    }
    else
    {
        // Lùi về phía sau
        single_1_e1(power, direct);
        single_1_e2(power, direct);
    }
}

// Động bộ dọc 2 động cơ bên trái
public void sync_2_Vertical_L(bool direct, int power )
{
    if (direct)
    {
        // Tiến về phía trước
        single_1_e3(power, direct);
        single_1_e4(power, direct);
    }
    else
    {
        // Lùi về phía sau
        single_1_e3(power, direct);
        single_1_e4(power, direct);
    }
}

// Động bộ ngang 2 động cơ bên phải
public void sync_2_Horizontal_R()
{
    return 0;
}

// Động bộ ngang 2 động cơ bên trái
public void sync_2_Horizontal_L()

{
    return 0;
}

// Nghịch đồng bộ 2 - 2 động cơ (Xoay tròn)
public void circular(bool isRight, int power)
{
    if (isRight)
    {
        // Quay về phía bên phải
        single_1_e1(power, 1);
        single_1_e2(power, 0);
        single_1_e3(power, 1);
        single_1_e4(power, 0);
    }
    else
    {
        // Quay về phía bên trái
        single_1_e1(power, 0);
        single_1_e2(power, 1);
        single_1_e3(power, 0);
        single_1_e4(power, 1);
    }
}

// Giao thức đặc biệt không qua các hàm xung
private void boot()
{
    return 0;
}
private void parking()
{
    return 0;
}
private void unParking()
{
    return 0;
}
private void pause()
{
    return 0;
}





/*============================================================================================================================================================================
Main Function
============================================================================================================================================================================*/
int main()
{
    while (true)
    {
        switch (STU)
        {
        case "1":
            /* code */
            break;
        
        default:
            break;
        }
    }
}