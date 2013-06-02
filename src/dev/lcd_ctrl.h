/**
 *******************************************************************************
 * @file    lcd_ctrl.h
 * @author  Olli Vanhoja
 * @brief   Header for lcd_ctrl.c module.
 *******************************************************************************
 */

/** @addtogroup Dev
  * @{
  */

/** @addtogroup lcd
  * @{
  */

#pragma once
#ifndef LCDCTRL_H
#define LCDCTRL_H

#include "queue.h"

extern queue_cb_t lcdc_queue_cb;

void lcdc_init(void);
void lcdc_init_thread(void);

#endif /* LCDCTRL_H */

/**
  * @}
  */

/**
  * @}
  */
