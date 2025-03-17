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
       if (direct)
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
    {
        return 0;
    }
    private void single_1_e3( float power, bool direct)
    {
        if (direct)
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
        if (direct)
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
public void sync_4( float power, bool direct)
{
// Xác thực phạm vi công suất
    if (power < 0 || power > 100) {
            return;
    }

// Kiểm tra tình trạng động cơ trước khi tiến hành
    if (!check_motor_status(e1) || !check_motor_status(e2) || 
        !check_motor_status(e3) || !check_motor_status(e4)) 
    {
        return;
    }

// Theo dõi tốc độ động cơ thực tế và điều chỉnh động
        float corrected_power_e1 = pid_control(target_speed, get_motor_speed(e1));
        float corrected_power_e2 = pid_control(target_speed, get_motor_speed(e2));
        float corrected_power_e3 = pid_control(target_speed, get_motor_speed(e3));
        float corrected_power_e4 = pid_control(target_speed, get_motor_speed(e4));
    
// Đồng bộ hóa tất cả các động cơ với công suất đã hiệu chỉnh
        single_1_e1(corrected_power_e1, direct);
        single_1_e2(corrected_power_e2, direct);
        single_1_e3(corrected_power_e3, direct);
        single_1_e4(corrected_power_e4, direct);
}

// Động bộ dọc 2 động cơ
// Đồng bộ hóa hai động cơ ở phía bên phải (căn chỉnh theo chiều dọc)
public void sync_2_Vertical_R(float power, bool direct)
{
    // Xác thực phạm vi công suất
    if (power < 0 || power > 100) {
        return;
    }

    // Kiểm tra xem động cơ có hoạt động không
    if (!check_motor_status(motor_R1) || !check_motor_status(motor_R2)) {
        return;
    }

    // Chuyển đổi năng lượng để điều khiển động cơ
    uint16_t converted_power = power_to_time_working(power);

    // Cấp nguồn cho động cơ
    if (direct) {
        // Hướng về phía trước
        motor_R1.in1 = converted_power;
        motor_R1.in2 = 0;
        motor_R2.in1 = converted_power;
        motor_R2.in2 = 0;
    }
    else {
        // Hướng ngược lại
        motor_R1.in1 = 0;
        motor_R1.in2 = converted_power;
        motor_R2.in1 = 0;
        motor_R2.in2 = converted_power;
    }

}

// Đồng bộ hai động cơ ở phía bên trái (căn chỉnh theo chiều dọc)
public void sync_2_Vertical_L(float power, bool direct)
{
// Xác thực phạm vi công suất
    if (power < 0 || power > 100) {
        return;
    }

// Kiểm tra xem động cơ có hoạt động không
    if (!check_motor_status(motor_L1) || !check_motor_status(motor_L2)) {
        return;
    }

// Chuyển đổi năng lượng để điều khiển động cơ
    uint16_t converted_power = power_to_time_working(power);

// Cấp nguồn cho động cơ
    if (direct) {
// Hướng về phía trước
        motor_L1.in1 = converted_power;
        motor_L1.in2 = 0;
        motor_L2.in1 = converted_power;
        motor_L2.in2 = 0;
    }
    else {
// Hướng ngược lại
        motor_L1.in1 = 0;
        motor_L1.in2 = converted_power;
        motor_L2.in1 = 0;
        motor_L2.in2 = converted_power;
    }

}

// Động bộ ngang 2 động cơ
// Đồng bộ hai động cơ ở phía bên phải (căn chỉnh theo chiều ngang)
public void sync_2_Horizontal_R(float power, bool direct)
{
    // Validate power range
    if (power < 0 || power > 100) {
        return;
    }

// Kiểm tra xem động cơ có hoạt động không
    if (!check_motor_status(motor_RH1) || !check_motor_status(motor_RH2)) {
        return;
    }

// Chuyển đổi năng lượng để điều khiển động cơ
    uint16_t converted_power = power_to_time_working(power);

// Cấp nguồn cho động cơ
    if (direct) {
// Hướng về phía trước
        motor_RH1.in1 = converted_power;
        motor_RH1.in2 = 0;
        motor_RH2.in1 = converted_power;
        motor_RH2.in2 = 0;
    }
    else {
// Hướng ngược lại
        motor_RH1.in1 = 0;
        motor_RH1.in2 = converted_power;
        motor_RH2.in1 = 0;
        motor_RH2.in2 = converted_power;
    }
}

// Đồng bộ hai động cơ ở phía bên trái (căn chỉnh theo chiều ngang)
public void sync_2_Horizontal_L(float power, bool direct)
{
// Xác thực phạm vi công suất
    if (power < 0 || power > 100) {
        return;
    }

// Kiểm tra xem động cơ có hoạt động không
    if (!check_motor_status(motor_LH1) || !check_motor_status(motor_LH2)) {
        return;
    }

// Chuyển đổi năng lượng để điều khiển động cơ
    uint16_t converted_power = power_to_time_working(power);

// Cấp nguồn cho động cơ
    if (direct) {
// Hướng về phía trước
        motor_LH1.in1 = converted_power;
        motor_LH1.in2 = 0;
        motor_LH2.in1 = converted_power;
        motor_LH2.in2 = 0;
    }
    else {
// Hướng ngược lại
        motor_LH1.in1 = 0;
        motor_LH1.in2 = converted_power;
        motor_LH2.in1 = 0;
        motor_LH2.in2 = converted_power;
    }
}

// Nghịch đồng bộ 2 - 2 động cơ (Xoay)
// Chuyển động ngược chiều của hai cặp động cơ (Chuyển động tròn)
public void circular(float power, bool direct)
{
// Xác thực phạm vi công suất
    if (power < 0 || power > 100) {
        return;
    }

// Kiểm tra xem cả bốn động cơ có hoạt động không trước khi di chuyển
    if (!check_motor_status(motor_1) || !check_motor_status(motor_2) || 
        !check_motor_status(motor_3) || !check_motor_status(motor_4)) {
        return;
    }

// Chuyển đổi năng lượng để điều khiển động cơ
    uint16_t converted_power = power_to_time_working(power);

    if (direct) {
// Quay theo chiều kim đồng hồ (CW)
        motor_1.in1 = converted_power;
        motor_1.in2 = 0;
        
        motor_2.in1 = 0;
        motor_2.in2 = converted_power;
        
        motor_3.in1 = converted_power;
        motor_3.in2 = 0;
        
        motor_4.in1 = 0;
        motor_4.in2 = converted_power;
    }
    else {
// Quay ngược chiều kim đồng hồ (CCW)
        motor_1.in1 = 0;
        motor_1.in2 = converted_power;
        
        motor_2.in1 = converted_power;
        motor_2.in2 = 0;
        
        motor_3.in1 = 0;
        motor_3.in2 = converted_power;
        
        motor_4.in1 = converted_power;
        motor_4.in2 = 0;
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