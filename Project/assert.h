#ifndef ASSERT_H
#define ASSERT_H

#define USE_FULL_ASSERT    1 


#ifdef  USE_FULL_ASSERT
  #define assert_param(expr) ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))
  void assert_failed(volatile uint8_t* file, volatile uint32_t line) __attribute__((optimize("-O0")));
#else
  #define assert_param(expr) ((void)0)
#endif /* USE_FULL_ASSERT */

#endif 
