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
    HJALMAR_NOT_SUPPORTED      = -1,
    HJALMAR_NOT_IMPLEMENTED    = -2,
    HJALMAR_INVALID_ARGUMENT   = -3,
    HJALMAR_QUEUE_FULL         = -4
} hjalmar_error_code_t;



#endif /* INC_ERROR_CODES_H_ */
