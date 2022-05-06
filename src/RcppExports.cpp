// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// get_test_landscape
Rcpp::DataFrame get_test_landscape(const int nItems, const float landsize, const int nClusters, const float clusterSpread, const int regen_time);
RcppExport SEXP _linkmoveecoevo_get_test_landscape(SEXP nItemsSEXP, SEXP landsizeSEXP, SEXP nClustersSEXP, SEXP clusterSpreadSEXP, SEXP regen_timeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const int >::type nItems(nItemsSEXP);
    Rcpp::traits::input_parameter< const float >::type landsize(landsizeSEXP);
    Rcpp::traits::input_parameter< const int >::type nClusters(nClustersSEXP);
    Rcpp::traits::input_parameter< const float >::type clusterSpread(clusterSpreadSEXP);
    Rcpp::traits::input_parameter< const int >::type regen_time(regen_timeSEXP);
    rcpp_result_gen = Rcpp::wrap(get_test_landscape(nItems, landsize, nClusters, clusterSpread, regen_time));
    return rcpp_result_gen;
END_RCPP
}
// model_case_2
S4 model_case_2(const int scenario, const int popsize, const int nItems, const float landsize, const int nClusters, const float clusterSpread, const int tmax, const int genmax, const float range_perception, const int handling_time, const int regen_time, const int nThreads, const float dispersal, const float mProb, const float mSize);
RcppExport SEXP _linkmoveecoevo_model_case_2(SEXP scenarioSEXP, SEXP popsizeSEXP, SEXP nItemsSEXP, SEXP landsizeSEXP, SEXP nClustersSEXP, SEXP clusterSpreadSEXP, SEXP tmaxSEXP, SEXP genmaxSEXP, SEXP range_perceptionSEXP, SEXP handling_timeSEXP, SEXP regen_timeSEXP, SEXP nThreadsSEXP, SEXP dispersalSEXP, SEXP mProbSEXP, SEXP mSizeSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const int >::type scenario(scenarioSEXP);
    Rcpp::traits::input_parameter< const int >::type popsize(popsizeSEXP);
    Rcpp::traits::input_parameter< const int >::type nItems(nItemsSEXP);
    Rcpp::traits::input_parameter< const float >::type landsize(landsizeSEXP);
    Rcpp::traits::input_parameter< const int >::type nClusters(nClustersSEXP);
    Rcpp::traits::input_parameter< const float >::type clusterSpread(clusterSpreadSEXP);
    Rcpp::traits::input_parameter< const int >::type tmax(tmaxSEXP);
    Rcpp::traits::input_parameter< const int >::type genmax(genmaxSEXP);
    Rcpp::traits::input_parameter< const float >::type range_perception(range_perceptionSEXP);
    Rcpp::traits::input_parameter< const int >::type handling_time(handling_timeSEXP);
    Rcpp::traits::input_parameter< const int >::type regen_time(regen_timeSEXP);
    Rcpp::traits::input_parameter< const int >::type nThreads(nThreadsSEXP);
    Rcpp::traits::input_parameter< const float >::type dispersal(dispersalSEXP);
    Rcpp::traits::input_parameter< const float >::type mProb(mProbSEXP);
    Rcpp::traits::input_parameter< const float >::type mSize(mSizeSEXP);
    rcpp_result_gen = Rcpp::wrap(model_case_2(scenario, popsize, nItems, landsize, nClusters, clusterSpread, tmax, genmax, range_perception, handling_time, regen_time, nThreads, dispersal, mProb, mSize));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_linkmoveecoevo_get_test_landscape", (DL_FUNC) &_linkmoveecoevo_get_test_landscape, 5},
    {"_linkmoveecoevo_model_case_2", (DL_FUNC) &_linkmoveecoevo_model_case_2, 15},
    {NULL, NULL, 0}
};

RcppExport void R_init_linkmoveecoevo(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
