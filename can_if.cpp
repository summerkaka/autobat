#include "app_include.h"

static int Phy_Init(void)
{

}

/**
 * @ingroup extern
 * @brief initial CAN socket, phy device
 * @param name : CAN device name, should be the same as ifconfig -a
 * @param bitrate : CAN bus bitrate
 * @return file descriptor of CAN socket if success
 * @return negativ error code if failed
 */
int CAN_Init(void)
{
    return 0;
}


