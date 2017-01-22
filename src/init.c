#include <R.h>
#include <Rinternals.h>
#include <R_ext/Rdynload.h>
#include <poppler.h>

#include "Rpoppler.h"

#define CALLDEF(name, n)  {#name, (DL_FUNC) &name, n}

static const R_CallMethodDef callMethods[] = {
    CALLDEF(Rpoppler_PDF_doc, 1),
    CALLDEF(Rpoppler_PDF_info, 1),
    CALLDEF(Rpoppler_PDF_fonts, 1),
    CALLDEF(Rpoppler_PDF_text, 1),

    {NULL, NULL, 0}
};

void
R_init_Rpoppler(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, callMethods, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
#if !GLIB_CHECK_VERSION(2, 36, 0)
    g_type_init();
#endif
    Rpoppler_set_error_function();
}
