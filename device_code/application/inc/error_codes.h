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

#ifndef INC_ERROR_CODES_H_
#define INC_ERROR_CODES_H_

typedef enum general_erros{
    HJALMAR_OK                 = 0,
    HJALMAR_FAILED             = -1,
    HJALMAR_BUSY               = -2,
    HJALMAR_TIMEOUT            = -3,
    HJALMAR_NOT_SUPPORTED      = -4,
    HJALMAR_NOT_IMPLEMENTED    = -5,
    HJALMAR_INVALID_ARGUMENT   = -6,
    HJALMAR_QUEUE_FULL         = -7,
    HJALMAR_HAL_ERROR         = -8
} hjalmar_error_code_t;



#endif /* INC_ERROR_CODES_H_ */
