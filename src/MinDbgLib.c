/* MinDbgLib.c */
#include <stdio.h>
/**
 * @param name The name of the macro, in a C style string.
 * @param value The expanded version of the macro, what its value actually is.
 */
typedef struct MDL_Macro {
    const char* name;
    void* value;
} MDL_Macro;

// Convert macro to string, by creating a `MDL_Macro` type.
#define MDL_GET_MACRO(macro) MDL_Macro {#macro, (void*)macro}
#define MDL_NULL_MACRO (MDL_Macro){0}

/**
 * Holds data related to the error.
 * @param description The description to the error, can be `NULL`.
 * @param macro The macro to go with the error, can be `NULL`.
 * @param code The value of the macro (not type dependant), cam be `NULL`.
 */
typedef struct MDL_Error {
    const char* description;
    const char* macro;
    void* code;
} MDL_Error;

#define MDL_EMPTY_ERROR (MDL_Error){0}

/**
 * The internal `MDL_Error` variable
 */
static MDL_Error mdl_internalErr = {0};
static MDL_Error mdl_emptyErr = {0};

/**
 * Sets the internal `mdl_internalErr` struct variables to be retrieved in `MDL_getError`.
 * @param desc The description of the error
 * @param macro The information about the macro, can be `NULL` with `MDL_NULL_MACRO`
 * @param code The value of the error, can be any type. For no value use `0`
 */
int MDL_throwError(const char* desc, MDL_Macro macro, void* code) {
    
    if(macro.name != 0) {
        printf("%s\n", desc);
        mdl_internalErr.description = desc;
        mdl_internalErr.macro = macro.name;
        mdl_internalErr.code = macro.value;
    } else {
        
        mdl_internalErr.description = desc;
        mdl_internalErr.macro = ((void*)0);
        mdl_internalErr.code = code;
        printf("123 %s\n", mdl_internalErr.description);
    }
    return 0;
}


/**
 * Sets the `target` parameter to `MDL_internalErr`.
 */
int MDL_getError(MDL_Error* target) {
    if(mdl_internalErr.description == 0) {
        return 0;
    }
    target = &mdl_internalErr;
    return 0;
}