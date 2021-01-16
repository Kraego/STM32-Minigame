# Arrow POC

To check Arrow, I sloppy added this code in the main.

``` C
// Arrow Display Demo
#include "debug.h"
#include "display.h"

  DEBUG_INIT(&huart2);
  DEBUG_PRINTF("starting demo!");
  display_Init();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();
    /* USER CODE BEGIN 3 */
    display_arrayWest();
    HAL_Delay(1000);
    display_arrayNorth();
    HAL_Delay(1000);
    display_arrayEast();
    HAL_Delay(1000);
    display_arraySouth();
    HAL_Delay(1000);
  }
  /* USER CODE END 3 */
```