# check function
Rcpp::compileAttributes()
devtools::build()
devtools::load_all()
# devtools::install()
devtools::document()

a = linkmoveecoevo::model_case_2(
  scenario = 0,
  landsize = 60,
  tmax = 20,
  handling_time = 5,
  dispersal = 3,
  popsize = 50,
  nClusters = 100,
  genmax = 1,
  regen_time = 100,
  mProb = 0.01,
  nItems = 1800,
  clusterSpread = 1.0,
  range_perception = 1.0,
  nThreads = 1,
  mSize = 0.01
)
