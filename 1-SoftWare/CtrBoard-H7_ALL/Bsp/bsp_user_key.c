#include "bsp_user_key.h"

KeyState keyState = NO_DETECT;
uint32_t overTick = 0;

int8_t BSP_UserKey_Detect(void)
{
    GPIO_PinState pinSta;
    uint32_t nowTick = HAL_GetTick();
    pinSta = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_15);
    if (keyState == NO_DETECT)
    {
        if (pinSta == GPIO_PIN_RESET)
        {
            keyState = DETECTING;
            #warning "TODO: 需要解决溢出的情况"
            overTick = nowTick + 100;//0.1s
            
        }
        return BUTTON_NOT_PRESSED;
    }
    else if (keyState == DETECTING)
    {
        if (nowTick >= overTick)
        {
            if (pinSta == GPIO_PIN_RESET)
            {
                keyState = DETECTED;
                return BUTTON_PRESSED;
            }
            else
            {
                keyState = NO_DETECT;
                return BUTTON_NOT_PRESSED;
            }
        }
        return BUTTON_NOT_PRESSED;
    }
    else if (keyState == DETECTED)
    {
        if (pinSta == GPIO_PIN_SET)
        {
            keyState = NO_DETECT;
        }
        return BUTTON_NOT_PRESSED;
    }
    return BUTTON_NOT_PRESSED;
}




