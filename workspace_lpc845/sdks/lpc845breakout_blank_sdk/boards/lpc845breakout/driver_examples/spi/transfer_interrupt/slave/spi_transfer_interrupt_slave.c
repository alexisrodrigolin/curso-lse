/*
 * Copyright  2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "fsl_spi.h"
#include "board.h"
#include "app.h"
#include "fsl_debug_console.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void slaveCallback(SPI_Type *base, spi_slave_handle_t *slaveHandle, status_t status, void *userData);
static void EXAMPLE_SlaveInit(void);
static void EXAMPLE_SlaveStartTransfer(void);
static void EXAMPLE_TransferDataCheck(void);

/*******************************************************************************
 * Variables
 ******************************************************************************/
#define BUFFER_SIZE (64)
spi_slave_handle_t slaveHandle;
static uint8_t rxBuffer[BUFFER_SIZE];
static uint8_t txBuffer[BUFFER_SIZE];
static volatile bool slaveFinished = false;
/*******************************************************************************
 * Code
 ******************************************************************************/
static void slaveCallback(SPI_Type *base, spi_slave_handle_t *slaveHandle, status_t status, void *userData)
{
    slaveFinished = true;
}

int main(void)
{
    /* Initialize the hardware. */
    BOARD_InitHardware();

    PRINTF("This is SPI interrupt transfer slave example.\n\r");
    PRINTF("\n\rSlave is working....\n\r");

    /* Initialize the slave SPI with configuration. */
    EXAMPLE_SlaveInit();

    /* Slave start transfer with master. */
    EXAMPLE_SlaveStartTransfer();

    /* Check transfer data. */
    EXAMPLE_TransferDataCheck();

    /* De-initialize the SPI. */
    SPI_Deinit(EXAMPLE_SPI_SLAVE);

    while (1)
    {
    }
}

static void EXAMPLE_SlaveInit(void)
{
    spi_slave_config_t userConfig;
    /* Default configuration for slave:
     * userConfig.enableSlave = true;
     * userConfig.polarity = kSPI_ClockPolarityActiveHigh;
     * userConfig.phase = kSPI_ClockPhaseFirstEdge;
     * userConfig.direction = kSPI_MsbFirst;
     * userConfig.dataWidth = kSPI_Data8Bits;
     * userConfig.sselPol = kSPI_SpolActiveAllLow;
     */
    SPI_SlaveGetDefaultConfig(&userConfig);
    SPI_SlaveInit(EXAMPLE_SPI_SLAVE, &userConfig);
}

static void EXAMPLE_SlaveStartTransfer(void)
{
    spi_transfer_t xfer = {0};
    uint32_t i          = 0U;

    /* Initialize the txBuffer and rxBuffer. */
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        txBuffer[i] = i;
        rxBuffer[i] = 0U;
    }

    SPI_SlaveTransferCreateHandle(EXAMPLE_SPI_SLAVE, &slaveHandle, slaveCallback, NULL);

    /* Receive data from master. */
    xfer.txData   = txBuffer;
    xfer.rxData   = rxBuffer;
    xfer.dataSize = sizeof(txBuffer);
    SPI_SlaveTransferNonBlocking(EXAMPLE_SPI_SLAVE, &slaveHandle, &xfer);
}

static void EXAMPLE_TransferDataCheck(void)
{
    uint32_t i = 0U, err = 0U;

    /* Waiting for transfer complete, if transmission is completed, the slaveFinished
     * will be set to ture in slaveCallback.
     */
    while (slaveFinished != true)
    {
    }

    PRINTF("\n\rThe received data are:");
    for (i = 0; i < BUFFER_SIZE; i++)
    {
        /* Print 16 numbers in a line */
        if ((i & 0x0FU) == 0U)
        {
            PRINTF("\n\r");
        }
        PRINTF("  0x%02X", rxBuffer[i]);
        /* Check if data matched. */
        if (txBuffer[i] != rxBuffer[i])
        {
            err++;
        }
    }

    if (err == 0)
    {
        PRINTF("\n\rSlave interrupt transfer succeed!\n\r");
    }
    else
    {
        PRINTF("\n\rSlave interrupt transfer faild!\n\r");
    }
}
