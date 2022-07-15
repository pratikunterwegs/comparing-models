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

#### check for relatively patchy landscape ####

l = ecoevomove2::get_test_landscape(
  nItems = 1800,
  landsize = 60,
  nClusters = 60,
  clusterSpread = 0.8,
  regen_time = 50
)

ggplot(l)+
  geom_point(aes(x, y))

a <- ecoevomove2::model_case_2(
  landsize = 60,
  nItems = 1800,
  clusterSpread = 0.8,
  nClusters = 60,
  regen_time = 50,
  tmax = 200,
  handling_time = 5,
  popsize = 500,
  genmax = 1000,
  max_dispersal = 20,
  range_perception = 1,
  mProb = 0.01,
  mSize = 0.01,
  nThreads = 2,
  initial_diverse = FALSE
)

str(a)

traits = a@trait_data
data.table::setDT(traits)

ggplot(traits, aes(moved))+
  geom_histogram()

ggplot(traits, aes(dispersal))+
  geom_histogram()

ggplot(traits, aes(moved, dispersal))+
  geom_jitter()

ggplot(traits, aes(exp(w0)))+
  geom_histogram()

ecoevomove2::scale_prefs(traits)

# distribution of fine-scale movement weights
ggplot(traits)+
  geom_jitter(
    aes(sF, sH, fill = sN),
    size = 4,
    shape = 21
  )+
  geom_abline(
    slope = c(-1, 1), intercept = c(1, -1)
  )+
  colorspace::scale_fill_continuous_diverging(
    palette = "Blue-Red 2",
    rev = T,
    limits = c(-1, 1)
  )+
  coord_equal(
    xlim = c(-1, 1),
    ylim = c(-1, 1)
  )

# distribution of dispersal decision weights
ggplot(traits)+
  geom_jitter(
    aes(wF, wN, fill = w0, size = dispersal),
    shape = 21
  )+
  geom_abline(
    slope = c(-1, 1, 0), intercept = c(1, -1, 0)
  )+
  colorspace::scale_fill_continuous_sequential(
    palette = "Batlow"
  )+
  # colorspace::scale_fill_continuous_diverging(
  #   palette = "Blue-Red 2",
  #   rev = T,
  #   # limits = c(-1, 1)
  # )+
  coord_equal(
    xlim = c(-1, 1),
    ylim = c(-1, 1)
  )+
  labs(title = "dispersal weights")

# plot displacement
ggplot(a@trait_data) +
  geom_point(
    data = a@landscape,
    aes(x, y),
    col = "green"
  )+
  geom_segment(
    aes(
      x = x, y = y,
      xend = xn, yend = yn
    )
  )+
  coord_equal()

b <- make_network(a, 2)

plot_network(b) +
  scale_fill_viridis_c()
