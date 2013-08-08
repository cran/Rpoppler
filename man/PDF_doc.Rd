\name{PDF_doc}
\alias{PDF_doc}
\title{
  PDF document reference
}
\description{
  Create a reference to a Portable Document Format (PDF) file for use in
  subsequent information extraction from the file.
}
\usage{
PDF_doc(file)
}
\arguments{
  \item{file}{A character string giving the path to a PDF file.}
}
\value{
  A reference to a PDF file (external pointer object).
}
\examples{
file <- system.file(file.path("doc", "Sweave.pdf"), package = "utils")
doc <- PDF_doc(file)
## Can now use the reference for information extraction, avoiding
## the creation of new PopplerDocument objects when doing so.
PDF_info(doc)
PDF_fonts(doc)
}
