# LCD_GLASS_BSP_ARROWS

## Plan:

Adding needed 4 Arrows as Character to bsp
* Arrow North
* Arrow South
* Arrow West
* West East

* Since BSP not Building without Warnings and I building it with werror, I had to fix some warnings (Remove µ and ° (multi character which the display couldn't display anyway)).

## Starting Point

In BSP: stm32l476g_discovery_glass_lcd.c

``` C
LCD segment mapping:
--------------------
  -----A-----        _
  |\   |   /|   COL |_|
  F H  J  K B
  |  \ | /  |        _
  --G-- --M--   COL |_|
  |  / | \  |
  E Q  P  N C
  |/   |   \|        _
  -----D-----   DP  |_|

 An LCD character coding is based on the following matrix:
COM           0   1   2     3
SEG(n)      { E , D , P ,   N   }
SEG(n+1)    { M , C , COL , DP  }
SEG(23-n-1) { B , A , K ,   J   }
SEG(23-n)   { G , F , Q ,   H   }
with n positive odd number.

 The character 'A' for example is:
  -------------------------------
LSB   { 1 , 0 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
      { 1 , 1 , 0 , 0   }
MSB   { 1 , 1 , 0 , 0   }
      -------------------
  'A' =  F    E   0   0 hexa
```

* Above you see how "A" the value for is created
* Now use this to calculate values for arrows.

## Calculation

* Array West "<"
  * Segment 3: K & N
``` C

        -------------------------------
        LSB   { 0 , 0 , 0 , 1   }
              { 0 , 1 , 0 , 0   }
              { 1 , 0 , 1 , 0   }
        MSB   { 0 , 0 , 0 , 0   }
            -------------------
                0   0   4   1 hexa
```
* Array East ">"
  * Segment 4: H & Q
``` C

        -------------------------------
        LSB   { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
        MSB   { 0 , 0 , 1 , 1   }
            -------------------
                0   0   8   8 hexa
```
* Array North1 "|"
  * Segment 3: K
``` C

        -------------------------------
        LSB   { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
        MSB   { 0 , 0 , 0 , 1   }
            -------------------
                0   0   4   0 hexa
```
* Array North2 "!"
* Segment 4: H
``` C

        -------------------------------
        LSB   { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
        MSB   { 0 , 0 , 0 , 1   }
            -------------------
                0   0   0   8 hexa
```
* Array South1 "_"
  * Segment 3: N
``` C

        -------------------------------
        LSB   { 0 , 0 , 0 , 1   }
              { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
        MSB   { 0 , 0 , 0 , 0   }
            -------------------
                0   0   0   1 hexa
```
* Array South2 "#"
* Segment 4: Q
``` C

        -------------------------------
        LSB   { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
              { 0 , 0 , 0 , 0   }
        MSB   { 0 , 0 , 1 , 0   }
            -------------------
                0   0   8   0 hexa
```