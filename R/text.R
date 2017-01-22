PDF_text <-
function(file)
{
    x <- PDF_doc_from_file(file)

    y <- .Call(Rpoppler_PDF_text, x$doc)

    y

}
