
#' Get network data from `simulation_output` objects.
#'
#' @param object A `simulation_output` object.
#'
#' @return A `tidygraph` objects.
#' @export
make_network = function(object) {
    assertthat::assert_that(
        class(object) == "simulation_output",
        msg = "pmv get traits: object is not simulation output"
    )

    # handle edges
    edgelist = object@edge_list
    colnames(edgelist) = c("from", "to", "weight")
    edgelist = data.table::as.data.table(edgelist)
    
    edgelist = edgelist[from != to, ]
    edgelist$to = edgelist$to + 1
    edgelist$from = edgelist$from + 1

    # handle nodes
    nodes = object@trait_data
    nodes = data.table::as.data.table(nodes)
    nodes$id = seq(nrow(nodes))

    # make tidygraph objects
    g = tidygraph::tbl_graph(
        nodes = nodes,
        edges = edgelist,
        directed = FALSE
    )

    g
}
