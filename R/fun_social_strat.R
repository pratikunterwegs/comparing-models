#' Assign a social strategy
#'
#' @param df The dataframe with agent id and social weights.
#'
#' @return A dataframe with social strategy assigned.
#' @export
#' @import data.table
#'
get_social_strategy = function(df) {
  assertthat::assert_that(
    all(c("sH", "sN") %in% names(df)),
    msg = "get_social_strat: data does not have social weights"
  )
  data.table::setDT(df)
  df[, social_strat := data.table::fcase(
    (sH > 0 & sN > 0), "agent tracking",
    (sH > 0 & sN < 0), "handler tracking",
    (sH < 0 & sN > 0), "non-handler tracking",
    (sH < 0 & sN < 0), "agent avoiding"
  )][]
}

scale_prefs = function(df) {
  # make data.table
  data.table::setDT(df)
  # get movement strategy
  df[, c("sF", "sH", "sN") := lapply(
    .SD, function(tr_) {
      tr_ / (abs(sF) + abs(sH) + abs(sN))
    }
  ), .SDcols = c("sF", "sH", "sN")]

  df[, c("wF", "wH", "wN", "w0") := lapply(
    .SD, function(tr_) {
      tr_ / (abs(wF) + abs(wH) + abs(wN) + abs(w0))
    }
  ), .SDcols = c("wF", "wH", "wN", "w0")]
}
