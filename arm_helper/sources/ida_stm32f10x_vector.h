#ifndef __IDA_STM32F10X_VECTOR_H__
#define __IDA_STM32F10X_VECTOR_H__

#include "ida_sdk.h"

#include "ida_name_vector.h"

#define STM32F10X_COMMON_IRQ_COUNT        18

#define STM32F10X_LD_VL_IRQ_COUNT         38
#define STM32F10X_MD_VL_IRQ_COUNT         38
#define STM32F10X_HD_VL_IRQ_COUNT         43

#define STM32F10X_LD_IRQ_COUNT            25
#define STM32F10X_MD_IRQ_COUNT            25
#define STM32F10X_HD_IRQ_COUNT            42

#define STM32F10X_XL_IRQ_COUNT            42
#define STM32F10X_CL_IRQ_COUNT            68


extern vector_t stm32f10x_common_vectors[STM32F10X_COMMON_IRQ_COUNT];

extern vector_t stm32f10x_ld_vl_vectors[STM32F10X_LD_VL_IRQ_COUNT];
extern vector_t stm32f10x_md_vl_vectors[STM32F10X_MD_VL_IRQ_COUNT];
extern vector_t stm32f10x_hd_vl_vectors[STM32F10X_HD_VL_IRQ_COUNT];

extern vector_t stm32f10x_ld_vectors[STM32F10X_LD_IRQ_COUNT];
extern vector_t stm32f10x_md_vectors[STM32F10X_MD_IRQ_COUNT];
extern vector_t stm32f10x_hd_vectors[STM32F10X_HD_IRQ_COUNT];

extern vector_t stm32f10x_xl_vectors[STM32F10X_XL_IRQ_COUNT];
extern vector_t stm32f10x_cl_vectors[STM32F10X_CL_IRQ_COUNT];




#endif /* __IDA_STM32F10X_VECTOR_H__ */