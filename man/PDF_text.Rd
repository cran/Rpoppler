\name{PDF_text}
\alias{PDF_text}
\title{PDF text extraction}
\description{
  Extract text from a Portable Document Format (PDF) file.
}
\usage{
PDF_text(file)
}
\arguments{
  \item{file}{A character string giving the path to a PDF file, or an
    object of class \code{"\link{PDF_doc}"} giving a reference to a PDF
    file.}
}
\value{
  A character vector with the extracted texts for each page.
}
\examples{
file <- system.file(file.path("doc", "Sweave.pdf"), package = "utils")
PDF_text(file)
}
