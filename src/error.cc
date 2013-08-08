#include <R.h>

#include "Rpoppler.h"

#ifdef HAVE_SET_ERROR_FUNCTION
void setErrorFunction(void (* f)(int , char *, va_list args));
#endif

#ifdef HAVE_SET_ERROR_CALLBACK
void setErrorCallback(void (*cbk)(void *data, ErrorCategory category,
				  int pos, char *msg),
		      void *data);
#endif

extern "C" {
void Rpoppler_set_error_function () {
#ifdef HAVE_SET_ERROR_FUNCTION    
    setErrorFunction( Rpoppler_error_function );
#endif
#ifdef HAVE_SET_ERROR_CALLBACK
    setErrorCallback( Rpoppler_error_callback, NULL );
#endif
}
}
