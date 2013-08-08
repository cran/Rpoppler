\name{PDF_info}
\alias{PDF_info}
\title{PDF document information}
\description{
  Extract document information from a Portable Document Format (PDF)
  file. 
}
\usage{
PDF_info(file)
}
\arguments{
  \item{file}{A character string giving the path to a PDF file, or an
    object of class \code{"\link{PDF_doc}"} giving a reference to a PDF
    file.}
}
\value{
  An object of class \code{PDF_info} (which has useful format and print
  methods), containing the information in the PDF Info dictionary
  (title, subject, keywords, author, creator, producer, creation date,
  modification date) as well as the number of pages and the page sizes, 
  whether the document is optimized (linearized), and the PDF version it
  uses.
}
\examples{
file <- system.file(file.path("doc", "Sweave.pdf"), package = "utils")
PDF_info(file)
}
