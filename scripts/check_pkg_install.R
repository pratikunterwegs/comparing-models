# check function
Rcpp::compileAttributes()
# devtools::build()
# devtools::load_all()
{
  sink("install_log.log")
  devtools::install(build = T, upgrade = "never")
  sink()
}
devtools::document()

library(ggplot2)

a <- ecoevomove2::model_case_2(
  landsize = 60,
  nItems = 1800,
  clusterSpread = 1.0,
  nClusters = 100,
  regen_time = 100,
  tmax = 20,
  handling_time = 5,
  popsize = 500,
  genmax = 100,
  dispersal = 0.01,
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
