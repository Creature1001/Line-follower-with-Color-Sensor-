///**
// * @file main.c
// *
// * @brief Main source code for the Line_Follower program.
// */
//
//#include "msp.h"
//#include "inc/Clock.h"
//#include "inc/CortexM.h"
//#include "inc/Gpio.h"
//#include "inc/EUSCI_A0_UART.h"
//#include "inc/Motor.h"
//#include "inc/Timer_A1_Interrupt.h"
//#include "inc/SysTick_Interrupt.h"
//#include "inc/Reflectance_Sensor.h"
//#include "inc/PmodColor.h"
//
//#define PwM_NOMINAL 2500
//
//static uint32_t Timer_A1_ms_elapsed = 0;
//static PmodColor_Data_t g_color_data;
//static volatile Detected_Color_t g_detected_color = COLOR_UNKNOWN;
//
//typedef enum
//{
//    CENTER                = 0b00011000,
//    SLIGHT_LEFT_CENTER    = 0b00011100,
//    SLIGHT_RIGHT_CENTER   = 0b00111000,
//    FAR_LEFT              = 0b00000001,
//    FAR_RIGHT             = 0b10000000,
//    LEFT_TURN             = 0b11111000,
//    RIGHT_TURN            = 0b00011111,
//    T_INTERSECTION        = 0b11111111,
//    DEAD_END              = 0b00000000
//} Line_Position;
//
//void Detect_Line_Position(uint8_t reflectance_sensor_data)
//{
//    switch(reflectance_sensor_data)
//    {
//        case CENTER:
//        {
//            Motor_Forward(1500, 1500);
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        case SLIGHT_LEFT_CENTER:
//        {
//            Motor_Forward(1500, 1500);
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        case SLIGHT_RIGHT_CENTER:
//        {
//            Motor_Forward(1500, 1500);
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        case FAR_LEFT:
//        {
//            Motor_Right(1500, 1500);
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        case FAR_RIGHT:
//        {
//            Motor_Left(1500, 1500);
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        case LEFT_TURN:
//        {
//            Motor_Left(1500, 1500);
//            LED1_Output(RED_LED_ON);
//        }
//        break;
//
//        case RIGHT_TURN:
//        {
//            Motor_Right(1500, 1500);
//            LED1_Output(RED_LED_ON);
//        }
//        break;
//
//        case T_INTERSECTION:
//        {
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        case DEAD_END:
//        {
//            Motor_Stop();
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//
//        default:
//        {
//            LED1_Output(RED_LED_OFF);
//        }
//        break;
//    }
//}
//
//void Timer_A1_Periodic_Task(void)
//{
//    Timer_A1_ms_elapsed++;
//
//    // Reflectance sensor update every 10 ms
//    if ((Timer_A1_ms_elapsed % 10) == 0)
//    {
//        Reflectance_Sensor_Start();
//    }
//
//    if ((Timer_A1_ms_elapsed % 10) == 1)
//    {
//        uint8_t Reflectance_Sensor_Data = Reflectance_Sensor_End();
//        Detect_Line_Position(Reflectance_Sensor_Data);
//    }
//
//    // Color sensor update every 100 ms
//    if ((Timer_A1_ms_elapsed % 100) == 0)
//    {
//        if (PmodColor_ReadRaw(&g_color_data))
//        {
//            g_detected_color = PmodColor_GetDetectedColor(&g_color_data);
//
//            switch (g_detected_color)
//            {
//                case COLOR_RED:
//                    LED2_Output(RGB_LED_RED);
//                    break;
//
//                case COLOR_GREEN:
//                    LED2_Output(RGB_LED_GREEN);
//                    break;
//
//                case COLOR_BLUE:
//                    LED2_Output(RGB_LED_BLUE);
//                    break;
//
//                case COLOR_WHITE:
//                    LED2_Output(RGB_LED_WHITE);
//                    break;
//
//                case COLOR_BLACK:
//                    LED2_Output(RGB_LED_OFF);
//                    break;
//
//                default:
//                    LED2_Output(RGB_LED_YELLOW);
//                    break;
//            }
//        }
//    }
//}
//
//int main(void)
//{
//    DisableInterrupts();
//
//    Clock_Init48MHz();
//
//    LED1_Init();
//    LED2_Init();
//
//    EUSCI_A0_UART_Init_Printf();
//
//    Reflectance_Sensor_Init();
//    PmodColor_Init();
//
//    SysTick_Interrupt_Init(SYSTICK_INT_NUM_CLK_CYCLES, SYSTICK_INT_PRIORITY);
//    Timer_A1_Interrupt_Init(&Timer_A1_Periodic_Task, TIMER_A1_INT_CCR0_VALUE);
//
//    Motor_Init();
//
//    EnableInterrupts();
//
//    while(1)
//    {
//
//    }
//}



// Color Test
#include "msp.h"
#include "inc/Clock.h"
#include "inc/CortexM.h"
#include "inc/Gpio.h"
#include "inc/EUSCI_A0_UART.h"
#include "inc/PmodColor.h"

static PmodColor_Data_t g_color_data;
static Detected_Color_t g_detected_color;

int main(void)
{
   DisableInterrupts();

   Clock_Init48MHz();
   LED1_Init();
   LED2_Init();
   EUSCI_A0_UART_Init_Printf();
   PmodColor_Init();

   EnableInterrupts();

   while (1)
   {
       if (PmodColor_ReadRaw(&g_color_data))
       {
           g_detected_color = PmodColor_GetDetectedColor(&g_color_data);

           printf("C=%u R=%u G=%u B=%u  -> ",
                  g_color_data.clear,
                  g_color_data.red,
                  g_color_data.green,
                  g_color_data.blue);

           switch (g_detected_color)
           {
               case COLOR_RED:
                   printf("RED\r\n");
                   break;

               case COLOR_GREEN:
                   printf("GREEN\r\n");
                   break;

               case COLOR_BLUE:
                   printf("BLUE\r\n");
                   break;

               case COLOR_WHITE:
                   printf("WHITE\r\n");
                   break;

               case COLOR_BLACK:
                   printf("BLACK\r\n");
                   break;

               default:
                   printf("UNKNOWN\r\n");
                   break;
           }
       }
       else
       {
           printf("Read failed\r\n");
       }

       Clock_Delay1ms(200);
   }
}
