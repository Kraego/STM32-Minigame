# Heading POC

To check Heading, I sloppy added this code in the main.

``` C
  // Compass Demo - TODO: remove
#include "compass.h"
#include "debug.h"
#include "heading_calc.h"

  DEBUG_INIT(&huart2);
  compass_Init();
  int16_t magn_vals[3];
  double magn[3];
  compass_GetRawValues(magn_vals);
  DEBUG_PRINTF("Raw Values x: %i, y: %i, z: %i", magn_vals[0], magn_vals[1], magn_vals[2]);

  compass_Calibrate();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    MX_USB_HOST_Process();
    compass_GetValues(magn);
    DEBUG_PRINTF("Magn Values x: %i, y: %i, z: %i", magn[0], magn[1], magn[2]);
    DEBUG_PRINTF("heading is %d",(int32_t) heading_Calc_getHeading(magn[0], magn[1]));
    DEBUG_PRINTF("\033[3A");
    HAL_Delay(1000);
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
```