#pragma once

/**
 * @brief Error codes retured by yloc functions.
 * 
 */
typedef enum : int {
    YLOC_STATUS_SUCCESS = 0,
    YLOC_STATUS_INVALID_ARGS,
    YLOC_STATUS_NOT_SUPPORTED,
    YLOC_STATUS_OUT_OF_RESOURCES,
    YLOC_STATUS_INTERNAL_EXCEPTION,
    YLOC_STATUS_INPUT_OUT_OF_BOUNDS,
    YLOC_STATUS_INIT_ERROR,
    YLOC_STATUS_NOT_YET_IMPLEMENTED,
    YLOC_STATUS_NOT_FOUND,
    YLOC_STATUS_INSUFFICIENT_SIZE,
    // YLOC_STATUS_INTERRUPT,
    YLOC_STATUS_UNEXPECTED_SIZE,
    YLOC_STATUS_NO_DATA,
    // YLOC_STATUS_UNEXPECTED_DATA,
    // YLOC_STATUS_BUSY,
    // YLOC_STATUS_REFCOUNT_OVERFLOW,
    YLOC_STATUS_UNKNOWN_ERROR
} yloc_status_t;
