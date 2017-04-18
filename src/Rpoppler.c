#include "Rpoppler.h"

static void PDF_doc_finalizer(SEXP ptr);

static char *PopplerFontTypeNames[] = {
    "unknown",
    "Type 1",
    "Type 1C",
    "Type 1C (OT)",
    "Type 3",
    "TrueType",
    "TrueType (OT)",
    "CID Type 0",
    "CID Type 0C",
    "CID Type 0C (OT)",
    "CID TrueType",
    "CID TrueType (OT)"
};

static char **Rpoppler_error_array = NULL;
static int Rpoppler_error_array_size = 0;
static int Rpoppler_error_count = 0;

static void Rpoppler_error_array_init()
{
    int n = 100;
    Rpoppler_error_count = 0;
    Rpoppler_error_array = Calloc(n, char *);
    Rpoppler_error_array_size = n;
}

static void Rpoppler_error_array_free()
{
    int i;
    for(i = 0; i < Rpoppler_error_count; i++) {
	Free(Rpoppler_error_array[i]);
    }
    Free(Rpoppler_error_array);
    Rpoppler_error_array = NULL;
    Rpoppler_error_count = 0;
    Rpoppler_error_array_size = 0;
}

static SEXP Rpoppler_error_array_to_sexp ()
{
    SEXP ans;
    int i;

    PROTECT(ans = allocVector(STRSXP, Rpoppler_error_count));
    for(i = 0; i < Rpoppler_error_count; i++) {
	SET_STRING_ELT(ans, i, mkChar(Rpoppler_error_array[i]));
    }
    UNPROTECT(1);

    return ans;
}

SEXP Rpoppler_PDF_doc(SEXP name)
{
    SEXP ptr;
    PopplerDocument *doc;
    GError *err;

    err = NULL;
    doc = poppler_document_new_from_file(CHAR(STRING_ELT(name, 0)),
					 NULL,
					 &err);
    if(doc == NULL) {
	error("[poppler] %s\n", err->message);
    } else {
	ptr = R_MakeExternalPtr(doc, install("Poppler_Document"),
				R_NilValue);
	PROTECT(ptr);
	R_RegisterCFinalizerEx(ptr, PDF_doc_finalizer, TRUE);
	UNPROTECT(1);
	return ptr;
    }
}

static void PDF_doc_finalizer(SEXP ptr)
{
    void *doc = R_ExternalPtrAddr(ptr);
    if(doc) {
	g_object_unref(doc);
	R_ClearExternalPtr(ptr);
    }
}

SEXP Rpoppler_PDF_info(SEXP x)
{
    SEXP ans, nms, val, err;
    PopplerDocument *doc;
    PopplerPage *page;
    int i, j, n;
    char *s;
    double w, h;
    time_t t;

    doc = R_ExternalPtrAddr(x);

    PROTECT(ans = allocVector(VECSXP, 13));
    PROTECT(nms = allocVector(STRSXP, 13));

    /* The string property getters return a new allocated string with
     * the respective property, or NULL.
     */

    s = poppler_document_get_title(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
    SET_VECTOR_ELT(ans, 0, val);
    SET_STRING_ELT(nms, 0, mkChar("Title"));

    s = poppler_document_get_subject(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
    SET_VECTOR_ELT(ans, 1, val);
    SET_STRING_ELT(nms, 1, mkChar("Subject"));

    s = poppler_document_get_keywords(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
    SET_VECTOR_ELT(ans, 2, val);
    SET_STRING_ELT(nms, 2, mkChar("Keywords"));

    s = poppler_document_get_author(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
    SET_VECTOR_ELT(ans, 3, val);
    SET_STRING_ELT(nms, 3, mkChar("Author"));

    s = poppler_document_get_creator(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
    SET_VECTOR_ELT(ans, 4, val);
    SET_STRING_ELT(nms, 4, mkChar("Creator"));

    s = poppler_document_get_producer(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
    SET_VECTOR_ELT(ans, 5, val);
    SET_STRING_ELT(nms, 5, mkChar("Producer"));

    /* The date property getters return the respective dates or -1. */

    t = poppler_document_get_creation_date(doc);
    val = (t == -1) ? ScalarReal(NA_REAL) : ScalarReal((double) t);
    SET_VECTOR_ELT(ans, 6, val);
    SET_STRING_ELT(nms, 6, mkChar("CreationDate"));

    t = poppler_document_get_modification_date(doc);
    val = (t == -1) ? ScalarReal(NA_REAL) : ScalarReal((double) t);
    SET_VECTOR_ELT(ans, 7, val);
    SET_STRING_ELT(nms, 7, mkChar("ModDate"));

    /* We also get the page layout and mode and permissions properties
       using
         poppler_document_get_page_layout()
	 poppler_document_get_page_mode()
	 poppler_document_get_permissions()
       Not done for done (is this information very useful?).
    */

    n = poppler_document_get_n_pages(doc);
    SET_VECTOR_ELT(ans, 8, ScalarInteger(n));
    SET_STRING_ELT(nms, 8, mkChar("Pages"));

    PROTECT(val = allocVector(REALSXP, 2 * n));
    for(i = 0, j = 0; i < n; i++) {
	page = poppler_document_get_page(doc, i);
	poppler_page_get_size(page, &w, &h);
	REAL(val)[j] = w; j++;
	REAL(val)[j] = h; j++;
	g_object_unref(page);
    }
    SET_VECTOR_ELT(ans, 9, val);
    UNPROTECT(1);
    SET_STRING_ELT(nms, 9, mkChar("Sizes"));

    SET_VECTOR_ELT(ans, 10,
		   ScalarLogical(poppler_document_is_linearized(doc) ?
				 TRUE : FALSE));
    SET_STRING_ELT(nms, 10, mkChar("Optimized"));

    s = poppler_document_get_pdf_version_string(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);    
    SET_VECTOR_ELT(ans, 11, val);
    SET_STRING_ELT(nms, 11, mkChar("PDF_Version"));

    s = poppler_document_get_metadata(doc);
    val = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);    
    SET_VECTOR_ELT(ans, 12, val);
    SET_STRING_ELT(nms, 12, mkChar("Metadata"));

    setAttrib(ans, R_NamesSymbol, nms);

    SEXP serrors = PROTECT(Rpoppler_error_array_to_sexp());
    setAttrib(ans, install("errors"), serrors);
    Rpoppler_error_array_free();
    
    UNPROTECT(3);

    return ans;
}

SEXP Rpoppler_PDF_fonts(SEXP x)
{
    SEXP ans, tmp, val;
    PopplerDocument *doc;
    PopplerFontInfo *info;
    PopplerFontsIter *iter;
    PopplerFontType t;
    int i, n;
    const char *s;
    gboolean b;

    doc = R_ExternalPtrAddr(x);    
    n = poppler_document_get_n_pages(doc);
    info = poppler_font_info_new(doc);

    PROTECT(ans = R_NilValue);
    while(poppler_font_info_scan(info, n, &iter)) {
	if(!iter) continue;
	do {
	    PROTECT(val = allocVector(VECSXP, 6));
	    s = poppler_fonts_iter_get_name(iter);
	    tmp = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
	    SET_VECTOR_ELT(val, 0, tmp);
	    s = poppler_fonts_iter_get_full_name(iter);
	    tmp = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
	    SET_VECTOR_ELT(val, 1, tmp);
	    t = poppler_fonts_iter_get_font_type(iter);
	    SET_VECTOR_ELT(val, 2, mkString(PopplerFontTypeNames[t]));
	    /* FIXME: NOTYET
	       poppler_fonts_iter_get_substitute_name ()
	       Returns the name of the substitute font or NULL if font
	       is embedded.
	       Since 0.20.
	    */
	    /* poppler_fonts_iter_get_file_name ()
	       Returns the filename of the font or NULL if font is
	       embedded.
	    */
	    s = poppler_fonts_iter_get_file_name(iter);
	    tmp = (s == NULL) ? ScalarString(NA_STRING) : mkString(s);
	    SET_VECTOR_ELT(val, 3, tmp);
	    /* FIXME: NOTYET
	       poppler_fonts_iter_get_encoding ()
	       Returns the font encoding.
	       Since 0.20.
	    */
	    b = poppler_fonts_iter_is_embedded(iter);
	    SET_VECTOR_ELT(val, 4, ScalarLogical(b));
	    b = poppler_fonts_iter_is_subset(iter);
	    SET_VECTOR_ELT(val, 5, ScalarLogical(b));
	    ans = CONS(val, ans);	    
	    UNPROTECT(2); /* ans, val */
	    PROTECT(ans);
	} while(poppler_fonts_iter_next(iter));
	poppler_fonts_iter_free(iter);
    }
    poppler_font_info_free(info);

    PROTECT(ans = coerceVector(ans, VECSXP));

    SEXP serrors = PROTECT(Rpoppler_error_array_to_sexp());
    setAttrib(ans, install("errors"), serrors);
    Rpoppler_error_array_free();

    UNPROTECT(3);

    return ans;
}

SEXP Rpoppler_PDF_text(SEXP x)
{
    SEXP ans, tmp;
    PopplerDocument *doc;
    PopplerPage *page;
    int i, n;
    char *s;

    doc = R_ExternalPtrAddr(x);

    n = poppler_document_get_n_pages(doc);

    PROTECT(ans = allocVector(STRSXP, n));

    for(i = 0; i < n; i++) {
	page = poppler_document_get_page(doc, i);
	s = poppler_page_get_text(page);
	tmp = (s == NULL) ? NA_STRING : mkChar(s);
	SET_STRING_ELT(ans, i, tmp);
	g_object_unref(page);
    }

    SEXP serrors = PROTECT(Rpoppler_error_array_to_sexp());
    setAttrib(ans, install("errors"), serrors);
    Rpoppler_error_array_free();

    UNPROTECT(2);

    return ans;
}

void Rpoppler_error_function(int pos, char *msg, va_list args)
{
    int i, n1, n2, n3;
    char *s1, *s2, *s3;

    if(!Rpoppler_error_array) Rpoppler_error_array_init();
    
    i = Rpoppler_error_count++;
    if(Rpoppler_error_count > Rpoppler_error_array_size) {
	Rpoppler_error_array_size += 100;
	Rpoppler_error_array =
	    Realloc(Rpoppler_error_array,
		    Rpoppler_error_array_size,
		    char *);
    }

    if(pos >= 0) {
	n1 = snprintf(NULL, 0, "Error (%d): ", pos) + 1;
	s1 = Calloc(n1, char);
	snprintf(s1, n1, "Error (%d): ", pos);
    } else {
	n1 = 8;
	s1 = Calloc(n1, char);
	snprintf(s1, n1, "Error: ");
    }
    n2 = vsnprintf(NULL, 0, msg, args) + 1;
    s2 = Calloc(n2, char);
    vsnprintf(s2, n2, msg, args);
    n3 = n1 + n2 - 1;
    s3 = Calloc(n3, char);
    strcpy(s3, s1);
    strcat(s3, s2);
    Free(s1);
    Free(s2);

    Rpoppler_error_array[i] = s3;
}

void Rpoppler_error_callback(void *data, ErrorCategory category,
			     int pos, char *msg)
{

    int i, n;
    char *s;
    
    if(!Rpoppler_error_array) Rpoppler_error_array_init();
    
    i = Rpoppler_error_count++;
    if(Rpoppler_error_count > Rpoppler_error_array_size) {
	Rpoppler_error_array_size += 100;
	Rpoppler_error_array =
	    Realloc(Rpoppler_error_array,
		    Rpoppler_error_array_size,
		    char *);
    }

    if(pos >= 0) {
	n = snprintf(NULL, 0, "Error (%d): %s", pos, msg) + 1;
	s = Calloc(n, char);
	snprintf(s, n, "Error (%d): %s", pos, msg);
    } else {
	n = 8 + strlen(msg) + 1;
	s = Calloc(n, char);
	snprintf(s, n, "Error: %s", msg);
    }
    
    Rpoppler_error_array[i] = s;
}
