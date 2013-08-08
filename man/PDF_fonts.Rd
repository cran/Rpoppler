\name{PDF_fonts}
\alias{PDF_fonts}
\title{PDF font information}
\description{
  Obtain the fonts used in a Portable Document Format (PDF) file and
  further information about these fonts.
}
\usage{
PDF_fonts(file)
}
\arguments{
  \item{file}{A character string giving the path to a PDF file, or an
    object of class \code{"\link{PDF_doc}"} giving a reference to a PDF
    file.}
}
\value{
  A data frame inheriting from \code{PDF_fonts} (which has a useful
  print method), with the following variables:
  \item{name}{the full name of the font (character)}
  \item{type}{the font type (Type 1, Type 3, etc.; character)}
  \item{file}{the file name of the font (character; empty if the font
    is embedded)}
  \item{emb}{whether the font is embedded in the PDF file or not
    (logical)}
  \item{sub}{whether the font is a subset of another font (logical)}
}
\examples{
file <- system.file(file.path("doc", "Sweave.pdf"), package = "utils")
PDF_fonts(file)
}
