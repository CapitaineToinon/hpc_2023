library("parallel")
library("foreach")
library("doParallel")

get_matrix_para <- function(nRow, nCol) {  
  X <- foreach(i=1:nRow, .combine='cbind') %dopar% {
    df <- as.data.frame(runif(n=nCol, min=0, max=9))
    names(df) <- paste0("col_",i)
    df
  }
  return(X)
}

compute_median <- function(data, numCores) {
  data_median <- mclapply(data, median, mc.cores=numCores)
  row <- t(as.matrix(data_median))
  return(row)
}

# initialize parallel "cluster"
numCores <- parallel::detectCores()
registerDoParallel(numCores)

data <- get_matrix_para(10000, 10000)
medians <- compute_median(data, numCores)
print(medians)

