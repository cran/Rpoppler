#ifndef _RPOPPLER_H
#define _RPOPPLER_H

#include <R.h>
#include <Rdefines.h>
#include <poppler.h>

#include "config.h"

typedef enum ErrorCategory {
    errSyntaxWarning
} ErrorCategory;

SEXP PDF_doc(SEXP name);
SEXP PDF_fonts(SEXP x);
SEXP PDF_info(SEXP x);
SEXP PDF_text(SEXP x);

#ifdef __cplusplus
extern "C" {
#endif
    void Rpoppler_error_function(int pos, char *msg, va_list args);
    void Rpoppler_error_callback(void *data, ErrorCategory category,
				 int pos, char *msg);
    void Rpoppler_set_error_function();
#ifdef __cplusplus
}
#endif

#endif
