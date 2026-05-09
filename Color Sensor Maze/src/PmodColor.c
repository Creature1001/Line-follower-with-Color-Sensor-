#include "msp.h"
#include "inc/Clock.h"
#include "inc/PmodColor.h"

#define PMODCOLOR_I2C_ADDR          0x29

#define PMODCOLOR_CMD_ENABLE        0xA0
#define PMODCOLOR_CMD_CDATAL_AUTO   0xB4

#define PMODCOLOR_ENABLE_PON        0x01
#define PMODCOLOR_ENABLE_AEN        0x02

#define I2C_TIMEOUT                 100000

static void I2C_B1_Init(void);
static bool I2C_B1_WriteByte(uint8_t slave_addr, uint8_t reg_cmd, uint8_t data);
static bool I2C_B1_ReadMulti(uint8_t slave_addr, uint8_t reg_cmd, uint8_t *buffer, uint8_t count);

void PmodColor_Init(void)
{
    I2C_B1_Init();

    // Power on
    I2C_B1_WriteByte(PMODCOLOR_I2C_ADDR, PMODCOLOR_CMD_ENABLE, PMODCOLOR_ENABLE_PON);
    Clock_Delay1ms(3);

    // Power on + ADC enable
    I2C_B1_WriteByte(PMODCOLOR_I2C_ADDR, PMODCOLOR_CMD_ENABLE,
                     PMODCOLOR_ENABLE_PON | PMODCOLOR_ENABLE_AEN);
    Clock_Delay1ms(3);
}

bool PmodColor_ReadRaw(PmodColor_Data_t *data)
{
    uint8_t raw[8];

    if (data == 0)
    {
        return false;
    }

    if (!I2C_B1_ReadMulti(PMODCOLOR_I2C_ADDR, PMODCOLOR_CMD_CDATAL_AUTO, raw, 8))
    {
        return false;
    }

    data->clear = ((uint16_t)raw[1] << 8) | raw[0];
    data->red   = ((uint16_t)raw[3] << 8) | raw[2];
    data->green = ((uint16_t)raw[5] << 8) | raw[4];
    data->blue  = ((uint16_t)raw[7] << 8) | raw[6];

    return true;
}

Detected_Color_t PmodColor_GetDetectedColor(PmodColor_Data_t *data)
{
    if (data == 0)
    {
        return COLOR_UNKNOWN;
    }

    if (data->clear < 80)
    {
        return COLOR_BLACK;
    }

    if ((data->red > 200) && (data->green > 200) && (data->blue > 200))
    {
        uint16_t max = data->red;
        uint16_t min = data->red;

        if (data->green > max) max = data->green;
        if (data->blue  > max) max = data->blue;
        if (data->green < min) min = data->green;
        if (data->blue  < min) min = data->blue;

        if ((max - min) < 80)
        {
            return COLOR_WHITE;
        }
    }

    if ((data->red > data->green) && (data->red > data->blue))
    {
        return COLOR_RED;
    }

    if ((data->green > data->red) && (data->green > data->blue))
    {
        return COLOR_GREEN;
    }

    if ((data->blue > data->red) && (data->blue > data->green))
    {
        return COLOR_BLUE;
    }

    return COLOR_UNKNOWN;
}

static void I2C_B1_Init(void)
{
    // P6.4 = UCB1SCL, P6.5 = UCB1SDA
    P6->SEL0 |= 0x30;
    P6->SEL1 &= ~0x30;

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_SWRST;
    EUSCI_B1->CTLW0 = EUSCI_B_CTLW0_SWRST |
                      EUSCI_B_CTLW0_MODE_3 |
                      EUSCI_B_CTLW0_MST |
                      EUSCI_B_CTLW0_SYNC |
                      EUSCI_B_CTLW0_SSEL__SMCLK;

    // 48 MHz / 120 = 400 kHz
    EUSCI_B1->BRW = 120;

    EUSCI_B1->I2CSA = PMODCOLOR_I2C_ADDR;
    EUSCI_B1->IFG = 0;
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_SWRST;
}

static bool I2C_B1_WriteByte(uint8_t slave_addr, uint8_t reg_cmd, uint8_t data)
{
    uint32_t timeout;

    while (EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY) {}

    EUSCI_B1->I2CSA = slave_addr;
    EUSCI_B1->IFG &= ~(EUSCI_B_IFG_NACKIFG | EUSCI_B_IFG_TXIFG0);
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    timeout = I2C_TIMEOUT;
    while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0))
    {
        if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            return false;
        }
        if (--timeout == 0)
        {
            return false;
        }
    }
    EUSCI_B1->TXBUF = reg_cmd;

    timeout = I2C_TIMEOUT;
    while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0))
    {
        if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            return false;
        }
        if (--timeout == 0)
        {
            return false;
        }
    }
    EUSCI_B1->TXBUF = data;

    timeout = I2C_TIMEOUT;
    while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0))
    {
        if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            return false;
        }
        if (--timeout == 0)
        {
            return false;
        }
    }

    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;

    timeout = I2C_TIMEOUT;
    while (EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTP)
    {
        if (--timeout == 0)
        {
            return false;
        }
    }

    return true;
}

static bool I2C_B1_ReadMulti(uint8_t slave_addr, uint8_t reg_cmd, uint8_t *buffer, uint8_t count)
{
    uint8_t i;
    uint32_t timeout;

    if ((buffer == 0) || (count == 0))
    {
        return false;
    }

    while (EUSCI_B1->STATW & EUSCI_B_STATW_BBUSY) {}

    EUSCI_B1->I2CSA = slave_addr;
    EUSCI_B1->IFG &= ~(EUSCI_B_IFG_NACKIFG | EUSCI_B_IFG_TXIFG0 | EUSCI_B_IFG_RXIFG0);

    // Start write phase: send register pointer
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TR;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    timeout = I2C_TIMEOUT;
    while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0))
    {
        if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            return false;
        }
        if (--timeout == 0)
        {
            return false;
        }
    }
    EUSCI_B1->TXBUF = reg_cmd;

    timeout = I2C_TIMEOUT;
    while (!(EUSCI_B1->IFG & EUSCI_B_IFG_TXIFG0))
    {
        if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            return false;
        }
        if (--timeout == 0)
        {
            return false;
        }
    }

    // Repeated START, switch to receive
    EUSCI_B1->CTLW0 &= ~EUSCI_B_CTLW0_TR;
    EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTT;

    timeout = I2C_TIMEOUT;
    while (EUSCI_B1->CTLW0 & EUSCI_B_CTLW0_TXSTT)
    {
        if (EUSCI_B1->IFG & EUSCI_B_IFG_NACKIFG)
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
            return false;
        }
        if (--timeout == 0)
        {
            return false;
        }
    }

    for (i = 0; i < count; i++)
    {
        if (i == (count - 1))
        {
            EUSCI_B1->CTLW0 |= EUSCI_B_CTLW0_TXSTP;
        }

        timeout = I2C_TIMEOUT;
        while (!(EUSCI_B1->IFG & EUSCI_B_IFG_RXIFG0))
        {
            if (--timeout == 0)
            {
                return false;
            }
        }
        buffer[i] = EUSCI_B1->RXBUF;
    }

    return true;
}

