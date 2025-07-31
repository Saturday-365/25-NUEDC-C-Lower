#include "main.h"

uint16_t get_key_num(){

if    (HAL_GPIO_ReadPin(BTN1_GPIO_Port, BTN1_Pin)) return 4;
else return 0;
}
