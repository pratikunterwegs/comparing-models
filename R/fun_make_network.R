
#' Get network data from `simulation_output` objects.
#'
#' @param object A `simulation_output` object.
#' @param weight_threshold Weight below which edges are excluded.
#'
#' @return A `tidygraph` objects.
#' @export
make_network = function(object, weight_threshold) {
    assertthat::assert_that(
        class(object) == "simulation_output",
        msg = "pmv get traits: object is not simulation output"
    )

    # handle edges
    edgelist = object@edge_list
    colnames(edgelist) = c("from", "to", "weight")
    edgelist = data.table::as.data.table(edgelist)
    
    edgelist = edgelist[edgelist$from != edgelist$to, ]

    edgelist = edgelist[edgelist$weight >= weight_threshold, ]

    edgelist$to = edgelist$to + 1
    edgelist$from = edgelist$from + 1

    # handle nodes
    nodes = object@trait_data
    nodes = data.table::as.data.table(nodes)
    nodes$id = seq(nrow(nodes))

    # get movement strategy
    nodes[, c("sF", "sH", "sN") := lapply(
      .SD, function(tr_) {
        tr_ / (abs(sF) + abs(sH) + abs(sN))
      }
    ), .SDcols = c("sF", "sH", "sN")][]
    nodes[, social_strat := data.table::fcase(
        (sH > 0 & sN > 0), "agent tracking",
        (sH > 0 & sN < 0), "handler tracking",
        (sH < 0 & sN > 0), "non-handler tracking",
        (sH < 0 & sN < 0), "agent avoiding"
  )][]

    # make tidygraph objects
    g = tidygraph::tbl_graph(
        nodes = nodes,
        edges = edgelist,
        directed = FALSE
    )

    g
}
