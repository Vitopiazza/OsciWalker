#ifndef CGPIO_H
#define CGPIO_H 1

#include "stm32f30x.h"

#define GPIO_INPUT(GPIOx, num) GPIOx->MODER &= ~(0x03<<(num*2));
#define GPIO_OUTPUT(GPIOx, num) GPIOx->MODER &= ~(0x03<<(num*2)); GPIOx->MODER |= (0x01<<(num*2));
#define GPIO_ALTERNATE(GPIOx, num) GPIOx->MODER &= ~(0x03<<(num*2)); GPIOx->MODER |= (0x02<<(num*2));
#define GPIO_ANALOG(GPIOx, num) GPIOx->MODER |= (0x03<<(num*2));

#define GPIO_PUSHPULL(GPIOx, num) GPIOx->OTYPER &= ~(0x01<<num);
#define GPIO_OPENDRAIN(GPIOx, num) GPIOx->OTYPER |= (0x01<<num);

#define GPIO_SPEED_400K(GPIOx, num) GPIOx->OSPEEDR &= ~(0x03<<(num*2));
#define GPIO_SPEED_2M(GPIOx, num) GPIOx->OSPEEDR &= ~(0x03<<(num*2)); GPIOx->OSPEEDR |= (0x01<<(num*2));
#define GPIO_SPEED_10M(GPIOx, num) GPIOx->OSPEEDR &= ~(0x03<<(num*2)); GPIOx->OSPEEDR |= (0x02<<(num*2));
#define GPIO_SPEED_40M(GPIOx, num) GPIOx->OSPEEDR |= (0x03<<(num*2));

#define GPIO_NOPULL(GPIOx, num) GPIOx->PUPDR &= ~(0x03<<(num*2));
#define GPIO_PULLUP(GPIOx, num) GPIOx->PUPDR &= ~(0x03<<(num*2)); GPIOx->PUPDR |= (0x01<<(num*2));
#define GPIO_PULLDOWN(GPIOx, num) GPIOx->PUPDR &= ~(0x03<<(num*2)); GPIOx->PUPDR |= (0x02<<(num*2));

#define GPIO_SET(GPIOx, num) GPIOx->BSRRL |= (0x01<<(num));
#define GPIO_RESET(GPIOx, num) GPIOx->BSRRH |= (0x01<<(num));
#define GPIO_READ(GPIOx, num) (GPIOx->IDR & (0x01<<(num)))

#define GPIO_FUNCTION(GPIOx, num, type) GPIO_FUNCTION_HELPER(GPIOx, num, type)
#define GPIO_FUNCTION_HELPER(GPIOx, num, type) GPIO_FUNCTION_##num(GPIOx, num, type)

#define GPIO_FUNCTION_0(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_1(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_2(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_3(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_4(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_5(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_6(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)
#define GPIO_FUNCTION_7(GPIOx, num, type) GPIO_FUNCTION_L(GPIOx, num, type)

#define GPIO_FUNCTION_8(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_9(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_10(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_11(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_12(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_13(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_14(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)
#define GPIO_FUNCTION_15(GPIOx, num, type) GPIO_FUNCTION_H(GPIOx, num, type)

#define GPIO_FUNCTION_L(GPIOx, num, type) GPIOx->AFR[0] &= ~(0x0f<<(num*4)); GPIOx->AFR[0] |= (type<<(num*4));
#define GPIO_FUNCTION_H(GPIOx, num, type) GPIOx->AFR[1] &= ~(0x0f<<((num-8)*4)); GPIOx->AFR[1] |= (type<<(((num-8))*4));


#endif