# check function
Rcpp::compileAttributes()
devtools::build()
devtools::load_all()
devtools::install(build = T, upgrade = "never")
devtools::document()

library(ggplot2)

# test case 0
a <- ecoevomove2::model_case_2(
  scenario = 0,
  landsize = 60,
  nItems = 1800,
  clusterSpread = 1.0,
  nClusters = 100,
  regen_time = 100,
  tmax = 400,
  handling_time = 5,
  popsize = 500,
  genmax = 1,
  dispersal = 3,
  range_perception = 1.0,
  mProb = 0.01,
  mSize = 0.01,
  nThreads = 1
)

# test case 3
a <- ecoevomove2::model_case_2(
  scenario = 3,
  landsize = 60,
  nItems = 1800,
  clusterSpread = 1.0,
  nClusters = 100,
  regen_time = 100,
  tmax = 20,
  handling_time = 5,
  popsize = 500,
  genmax = 20,
  dispersal = 10,
  range_perception = 1.0,
  mProb = 0.01,
  mSize = 0.01,
  nThreads = 2
)

# test case 2
a <- ecoevomove2::model_case_2(
  scenario = 2,
  landsize = 60,
  nItems = 1800,
  clusterSpread = 1.0,
  nClusters = 100,
  regen_time = 100,
  tmax = 20,
  handling_time = 5,
  popsize = 500,
  genmax = 20,
  dispersal = 10,
  range_perception = 1.0,
  mProb = 0.01,
  mSize = 0.01,
  nThreads = 2
)

# test case 1
a <- ecoevomove2::model_case_2(
  scenario = 1,
  landsize = 60,
  nItems = 1800,
  clusterSpread = 1.0,
  nClusters = 100,
  regen_time = 100,
  tmax = 20,
  handling_time = 5,
  popsize = 500,
  genmax = 1,
  dispersal = 3,
  range_perception = 1.0,
  mProb = 0.01,
  mSize = 0.01,
  nThreads = 2
)

str(a)

# plot displacement
ggplot(a@trait_data) +
  geom_segment(
    aes(
      x = x, y = y,
      xend = xn, yend = yn
    )
  )

b <- make_network(a, 2)

plot_network(b) +
  scale_fill_viridis_c()
