/*
 * pin_placements.h
 *
 *  Created on: Apr 6, 2018
 *      Author: jaxc
 */

#ifndef STM32F4_DISCOVERY_INC_PIN_PLACEMENTS_H_
#define STM32F4_DISCOVERY_INC_PIN_PLACEMENTS_H_

#define USER_Btn_Pin GPIO_PIN_13
#define USER_Btn_GPIO_Port GPIOC
#define MCO_Pin GPIO_PIN_0
#define MCO_GPIO_Port GPIOH
#define LD3_Pin GPIO_PIN_14
#define LD3_GPIO_Port GPIOB
#define STLK_RX_Pin GPIO_PIN_8
#define STLK_RX_GPIO_Port GPIOD
#define STLK_TX_Pin GPIO_PIN_9
#define STLK_TX_GPIO_Port GPIOD
#define USB_PowerSwitchOn_Pin GPIO_PIN_6
#define USB_PowerSwitchOn_GPIO_Port GPIOG
#define MIDI_in_Pin GPIO_PIN_10
#define MIDI_in_GPIO_Port GPIOA
#define MIDI_out_Pin GPIO_PIN_6
#define MIDI_out_GPIO_Port GPIOB
#define LD2_Pin GPIO_PIN_7
#define LD2_GPIO_Port GPIOB

#endif /* STM32F4_DISCOVERY_INC_PIN_PLACEMENTS_H_ */
