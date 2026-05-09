#ifndef PMODCOLOR_H_
#define PMODCOLOR_H_

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    uint16_t clear;
    uint16_t red;
    uint16_t green;
    uint16_t blue;
} PmodColor_Data_t;

typedef enum
{
    COLOR_UNKNOWN = 0,
    COLOR_RED,
    COLOR_GREEN,
    COLOR_BLUE,
    COLOR_WHITE,
    COLOR_BLACK
} Detected_Color_t;

void PmodColor_Init(void);
bool PmodColor_ReadRaw(PmodColor_Data_t *data);
Detected_Color_t PmodColor_GetDetectedColor(PmodColor_Data_t *data);

#endif
