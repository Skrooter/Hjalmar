/*
 * error_codes.h
 *
 *  Created on: Oct 24, 2017
 *      Author: jaxc
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
