/*
 * This file is part of Hjalmar.
 *
 * Hjalmar is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Hjalmar is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Hjalmar.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INC_MIDI_CONSTANTS_H_
#define INC_MIDI_CONSTANTS_H_

#include <stdint.h>

typedef enum midi_message_type{
    MIDI_NOTE_OFF                = 0x8,
    MIDI_NOTE_ON                 = 0x9,
    MIDI_POLYPHONIC_PRESSURE     = 0xA,
    MIDI_CONTROL_CHANGE          = 0xB,
    MIDI_PROGRAM_CHANGE          = 0xC,
    MIDI_CHANNEL_PRESSURE        = 0xD,
    MIDI_PITCH_BEND              = 0xE,
    MIDI_SYSTEM                  = 0xF
} midi_message_type_t;

typedef enum midi_system_type{
    MIDI_SYSTEM_TIMING_CLOCK    = 0xF8,
    MIDI_SYSTEM_UNDEFINED       = 0xF9,
    MIDI_SYSTEM_START           = 0xFA,
    MIDI_SYSTEM_CONTINUE        = 0xFB,
    MIDI_SYSTEM_STOP            = 0xFC,
    MIDI_SYSTEM_UNDEFINED_2     = 0xFD,
    MIDI_SYSTEM_ACTIVE_SENSE    = 0xFE,
    MIDI_SYSTEM_PANIC           = 0xFF
} midi_system_type_t;

typedef enum octave_offset{
    OCTAVE_O            = 0,
    OCTAVE_1            = 12,
    OCTAVE_2            = 24,
    OCTAVE_3            = 36,
    OCTAVE_4            = 48,
    OCTAVE_5            = 60,
    OCTAVE_6            = 72,
    OCTAVE_7            = 84,
    OCTAVE_8            = 96,
    OCTAVE_9            = 108,
    OCTAVE_1O           = 120
} octave_offset_t;

typedef enum note_offset{
    NOTE_C              = 0,
    NOTE_DB             = 1,
    NOTE_D              = 2,
    NOTE_EB             = 3,
    NOTE_E              = 4,
    NOTE_F              = 5,
    NOTE_GB             = 6,
    NOTE_G              = 7,
    NOTE_AB             = 8,
    NOTE_A              = 9,
    NOTE_BB             = 10,
    NOTE_B              = 11
} note_offset_t;

typedef enum midi_note_number{
    NOTE_C_0            = OCTAVE_O + NOTE_C,
    NOTE_DB_0           = OCTAVE_O + NOTE_DB,
    NOTE_D_0            = OCTAVE_O + NOTE_D,
    NOTE_EB_0           = OCTAVE_O + NOTE_EB,
    NOTE_E_0            = OCTAVE_O + NOTE_E,
    NOTE_F_0            = OCTAVE_O + NOTE_F,
    NOTE_GB_0           = OCTAVE_O + NOTE_GB,
    NOTE_G_0            = OCTAVE_O + NOTE_G,
    NOTE_AB_0           = OCTAVE_O + NOTE_AB,
    NOTE_A_0            = OCTAVE_O + NOTE_A,
    NOTE_BB_0           = OCTAVE_O + NOTE_BB,
    NOTE_B_0            = OCTAVE_O + NOTE_B,
    NOTE_C_1            = OCTAVE_1 + NOTE_C,
    NOTE_DB_1           = OCTAVE_1 + NOTE_DB,
    NOTE_D_1            = OCTAVE_1 + NOTE_D,
    NOTE_EB_1           = OCTAVE_1 + NOTE_EB,
    NOTE_E_1            = OCTAVE_1 + NOTE_E,
    NOTE_F_1            = OCTAVE_1 + NOTE_F,
    NOTE_GB_1           = OCTAVE_1 + NOTE_GB,
    NOTE_G_1            = OCTAVE_1 + NOTE_G,
    NOTE_AB_1           = OCTAVE_1 + NOTE_AB,
    NOTE_A_1            = OCTAVE_1 + NOTE_A,
    NOTE_BB_1           = OCTAVE_1 + NOTE_BB,
    NOTE_B_1            = OCTAVE_1 + NOTE_B,
    NOTE_C_2            = OCTAVE_2 + NOTE_C,
    NOTE_DB_2           = OCTAVE_2 + NOTE_DB,
    NOTE_D_2            = OCTAVE_2 + NOTE_D,
    NOTE_EB_2           = OCTAVE_2 + NOTE_EB,
    NOTE_E_2            = OCTAVE_2 + NOTE_E,
    NOTE_F_2            = OCTAVE_2 + NOTE_F,
    NOTE_GB_2           = OCTAVE_2 + NOTE_GB,
    NOTE_G_2            = OCTAVE_2 + NOTE_G,
    NOTE_AB_2           = OCTAVE_2 + NOTE_AB,
    NOTE_A_2            = OCTAVE_2 + NOTE_A,
    NOTE_BB_2           = OCTAVE_2 + NOTE_BB,
    NOTE_B_2            = OCTAVE_2 + NOTE_B,
    NOTE_C_3            = OCTAVE_3 + NOTE_C,
    NOTE_DB_3           = OCTAVE_3 + NOTE_DB,
    NOTE_D_3            = OCTAVE_3 + NOTE_D,
    NOTE_EB_3           = OCTAVE_3 + NOTE_EB,
    NOTE_E_3            = OCTAVE_3 + NOTE_E,
    NOTE_F_3            = OCTAVE_3 + NOTE_F,
    NOTE_GB_3           = OCTAVE_3 + NOTE_GB,
    NOTE_G_3            = OCTAVE_3 + NOTE_G,
    NOTE_AB_3           = OCTAVE_3 + NOTE_AB,
    NOTE_A_3            = OCTAVE_3 + NOTE_A,
    NOTE_BB_3           = OCTAVE_3 + NOTE_BB,
    NOTE_B_3            = OCTAVE_3 + NOTE_B,
    NOTE_C_4            = OCTAVE_4 + NOTE_C,
    NOTE_DB_4           = OCTAVE_4 + NOTE_DB,
    NOTE_D_4            = OCTAVE_4 + NOTE_D,
    NOTE_EB_4           = OCTAVE_4 + NOTE_EB,
    NOTE_E_4            = OCTAVE_4 + NOTE_E,
    NOTE_F_4            = OCTAVE_4 + NOTE_F,
    NOTE_GB_4           = OCTAVE_4 + NOTE_GB,
    NOTE_G_4            = OCTAVE_4 + NOTE_G,
    NOTE_AB_4           = OCTAVE_4 + NOTE_AB,
    NOTE_A_4            = OCTAVE_4 + NOTE_A,
    NOTE_BB_4           = OCTAVE_4 + NOTE_BB,
    NOTE_B_4            = OCTAVE_4 + NOTE_B,
    NOTE_C_5            = OCTAVE_5 + NOTE_C,
    NOTE_DB_5           = OCTAVE_5 + NOTE_DB,
    NOTE_D_5            = OCTAVE_5 + NOTE_D,
    NOTE_EB_5           = OCTAVE_5 + NOTE_EB,
    NOTE_E_5            = OCTAVE_5 + NOTE_E,
    NOTE_F_5            = OCTAVE_5 + NOTE_F,
    NOTE_GB_5           = OCTAVE_5 + NOTE_GB,
    NOTE_G_5            = OCTAVE_5 + NOTE_G,
    NOTE_AB_5           = OCTAVE_5 + NOTE_AB,
    NOTE_A_5            = OCTAVE_5 + NOTE_A,
    NOTE_BB_5           = OCTAVE_5 + NOTE_BB,
    NOTE_B_5            = OCTAVE_5 + NOTE_B,
    NOTE_C_6            = OCTAVE_6 + NOTE_C,
    NOTE_DB_6           = OCTAVE_6 + NOTE_DB,
    NOTE_D_6            = OCTAVE_6 + NOTE_D,
    NOTE_EB_6           = OCTAVE_6 + NOTE_EB,
    NOTE_E_6            = OCTAVE_6 + NOTE_E,
    NOTE_F_6            = OCTAVE_6 + NOTE_F,
    NOTE_GB_6           = OCTAVE_6 + NOTE_GB,
    NOTE_G_6            = OCTAVE_6 + NOTE_G,
    NOTE_AB_6           = OCTAVE_6 + NOTE_AB,
    NOTE_A_6            = OCTAVE_6 + NOTE_A,
    NOTE_BB_6           = OCTAVE_6 + NOTE_BB,
    NOTE_B_6            = OCTAVE_6 + NOTE_B,
    NOTE_C_7            = OCTAVE_7 + NOTE_C,
    NOTE_DB_7           = OCTAVE_7 + NOTE_DB,
    NOTE_D_7            = OCTAVE_7 + NOTE_D,
    NOTE_EB_7           = OCTAVE_7 + NOTE_EB,
    NOTE_E_7            = OCTAVE_7 + NOTE_E,
    NOTE_F_7            = OCTAVE_7 + NOTE_F,
    NOTE_GB_7           = OCTAVE_7 + NOTE_GB,
    NOTE_G_7            = OCTAVE_7 + NOTE_G,
    NOTE_AB_7           = OCTAVE_7 + NOTE_AB,
    NOTE_A_7            = OCTAVE_7 + NOTE_A,
    NOTE_BB_7           = OCTAVE_7 + NOTE_BB,
    NOTE_B_7            = OCTAVE_7 + NOTE_B,
    NOTE_C_8            = OCTAVE_8 + NOTE_C,
    NOTE_DB_8           = OCTAVE_8 + NOTE_DB,
    NOTE_D_8            = OCTAVE_8 + NOTE_D,
    NOTE_EB_8           = OCTAVE_8 + NOTE_EB,
    NOTE_E_8            = OCTAVE_8 + NOTE_E,
    NOTE_F_8            = OCTAVE_8 + NOTE_F,
    NOTE_GB_8           = OCTAVE_8 + NOTE_GB,
    NOTE_G_8            = OCTAVE_8 + NOTE_G,
    NOTE_AB_8           = OCTAVE_8 + NOTE_AB,
    NOTE_A_8            = OCTAVE_8 + NOTE_A,
    NOTE_BB_8           = OCTAVE_8 + NOTE_BB,
    NOTE_B_8            = OCTAVE_8 + NOTE_B,
    NOTE_C_9            = OCTAVE_9 + NOTE_C,
    NOTE_DB_9           = OCTAVE_9 + NOTE_DB,
    NOTE_D_9            = OCTAVE_9 + NOTE_D,
    NOTE_EB_9           = OCTAVE_9 + NOTE_EB,
    NOTE_E_9            = OCTAVE_9 + NOTE_E,
    NOTE_F_9            = OCTAVE_9 + NOTE_F,
    NOTE_GB_9           = OCTAVE_9 + NOTE_GB,
    NOTE_G_9            = OCTAVE_9 + NOTE_G,
    NOTE_AB_9           = OCTAVE_9 + NOTE_AB,
    NOTE_A_9            = OCTAVE_9 + NOTE_A,
    NOTE_BB_9           = OCTAVE_9 + NOTE_BB,
    NOTE_B_9            = OCTAVE_9 + NOTE_B,
    NOTE_C_10           = OCTAVE_1O + NOTE_C,
    NOTE_DB_10          = OCTAVE_1O + NOTE_DB,
    NOTE_D_10           = OCTAVE_1O + NOTE_D,
    NOTE_EB_10          = OCTAVE_1O + NOTE_EB,
    NOTE_E_10           = OCTAVE_1O + NOTE_E,
    NOTE_F_10           = OCTAVE_1O + NOTE_F,
    NOTE_GB_10          = OCTAVE_1O + NOTE_GB,
    NOTE_G_10           = OCTAVE_1O + NOTE_G
} midi_note_number_t;

typedef enum midi_channel{
    MIDI_CHANNEL_0      = 0,
    MIDI_CHANNEL_1      = 1,
    MIDI_CHANNEL_2      = 2,
    MIDI_CHANNEL_3      = 3,
    MIDI_CHANNEL_4      = 4,
    MIDI_CHANNEL_5      = 5,
    MIDI_CHANNEL_6      = 6,
    MIDI_CHANNEL_7      = 7,
    MIDI_CHANNEL_8      = 8,
    MIDI_CHANNEL_9      = 9,
    MIDI_CHANNEL_10     = 10,
    MIDI_CHANNEL_11     = 11,
    MIDI_CHANNEL_12     = 12,
    MIDI_CHANNEL_13     = 13,
    MIDI_CHANNEL_14     = 14,
    MIDI_CHANNEL_15     = 15,

} midi_channel_t;

typedef struct midi_note_cmd{
    uint8_t             cmd_chan;
    uint8_t             note_number;
    uint8_t             velocity;
    uint8_t             dummy;
} midi_note_cmd_t;

typedef struct midi_poly_pressure {
    midi_message_type_t        command;
    midi_channel_t        channel;
    midi_note_number_t    note_number;
    uint8_t             pressure;
} midi_poly_pressure_t;

typedef struct midi_control_change {
    midi_message_type_t        command;
    midi_channel_t        channel;
    uint8_t             controller_number;
    uint8_t             control_change_value;
} midi_control_change_t;

typedef struct midi_program_change {
    midi_message_type_t        command;
    midi_channel_t        channel;
    uint8_t             program_number;
} midi_program_change_t;

typedef struct midi_channel_pressure {
    midi_message_type_t        command;
    midi_channel_t        channel;
    uint8_t             pressure;
} midi_channel_pressure_t;

typedef struct midi_pitch_bend {
    midi_message_type_t        command;
    midi_channel_t        channel;
    uint16_t            pitch_bend;
} midi_pitch_bend_t;

#endif /* INC_MIDI_CONSTANTS_H_ */
