
#include "ida_cortex_m3.h"

//------------------------------------------------------------------------------
//
typedef enum
{
  STM32F10X_LD_VL = 0, /* Low density Value Line devices */
  STM32F10X_MD_VL = 1, /* Medium density Value Line devices */
  STM32F10X_HD_VL = 2, /* High density value line devices */
  STM32F10X_LD = 3, /* Low density devices */
  STM32F10X_MD = 4, /* Medium density devices */
  STM32F10X_HD = 5 , /* High density devices */
  STM32F10X_XL = 6 , /* XL-density devices */
  STM32F10X_CL = 7 , /* Connectivity line devices */
  STM32F10X_TYPE_COUNT
} eSTM32F10X_TYPE;

class ida_stm32f10x :
  public ida_cortex_m3
{
    eSTM32F10X_TYPE type;
    int showConfirmDialog ( void );
    int showSelectDialog ( void );
    int selectType ( void );
  public:
    ida_stm32f10x ( void );
    ~ida_stm32f10x ( void );
    void nameRegisters ( void );
    void nameAllVectors ( void );
    void run ( void );
    int init ( void );
};
