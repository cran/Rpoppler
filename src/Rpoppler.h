#ifndef _RPOPPLER_H
#define _RPOPPLER_H

#include <R.h>
#include <Rdefines.h>
#include <poppler.h>

#include "config.h"

typedef enum ErrorCategory {
    errSyntaxWarning
} ErrorCategory;

SEXP Rpoppler_PDF_doc(SEXP name);
SEXP Rpoppler_PDF_fonts(SEXP x);
SEXP Rpoppler_PDF_info(SEXP x);
SEXP Rpoppler_PDF_text(SEXP x);

#ifdef __cplusplus
extern "C" {
#endif
    void Rpoppler_error_function(int pos, char *msg, va_list args);
    void Rpoppler_error_callback(void *data, ErrorCategory category,
				 int pos, char *msg);
    void Rpoppler_set_error_function(void);
#ifdef __cplusplus
}
#endif

#endif
