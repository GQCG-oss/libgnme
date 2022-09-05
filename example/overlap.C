#include <iostream>
#include <armadillo>
#include <cassert>
#include <libgnme/wick/wick_orbitals.h>
#include <libgnme/slater/slater_uscf.h>
#include <libgnme/wick/wick_uscf.h>
#include <iomanip>

using namespace libgnme;

int main () {

    // Filenames
    std::string fname_Cx_a = std::string("Cx_a.csv");
    std::string fname_Cx_b = std::string("Cx_b.csv");
    std::string fname_Cw_a = std::string("Cw_a.csv");
    std::string fname_Cw_b = std::string("Cw_b.csv");
    std::string fname_ovlp = std::string("ao_overlap.csv");
    std::string fname_hcore = std::string("ao_hcore.csv");

    // HARDCODED basis information
    size_t nbsf = 3, nocca = 2, noccb = 1, nmo = 3, nact = 3;
 
    // Read-in overlap matrix
    arma::mat S;
    if(!S.load(fname_ovlp, arma::csv_ascii)) return 1;
    assert(S.n_rows == nbsf);
    assert(S.n_cols == nbsf);

    // Read-in core Hamiltonian
    arma::mat hcore; 
    if(!hcore.load(fname_hcore, arma::csv_ascii)) return 1;
    assert(hcore.n_rows == nbsf);
    assert(hcore.n_cols == nbsf);

    // Read-in orbital coefficients ket
    arma::mat Cx_a;
    arma::mat Cx_b;
    if(!Cx_a.load(fname_Cx_a, arma::csv_ascii)) return 1;
    assert(Cx_a.n_rows == nbsf);
    assert(Cx_a.n_cols == nmo);
    if(!Cx_b.load(fname_Cx_b, arma::csv_ascii)) return 1;
    assert(Cx_b.n_rows == nbsf);
    assert(Cx_b.n_cols == nmo);

    // Read-in orbital coefficients bra
    arma::mat Cw_a;
    arma::mat Cw_b;
    if(!Cw_a.load(fname_Cw_a, arma::csv_ascii)) return 1;
    assert(Cw_a.n_rows == nbsf);
    assert(Cw_a.n_cols == nmo);
    if(!Cw_b.load(fname_Cw_b, arma::csv_ascii)) return 1;
    assert(Cw_b.n_rows == nbsf);
    assert(Cw_b.n_cols == nmo);

    // Construct the wick_orbitals object
    wick_orbitals<double,double> orbs_a(nbsf, nmo, nocca, Cx_a, Cw_a, S);
    wick_orbitals<double,double> orbs_b(nbsf, nmo, noccb, Cx_b, Cw_b, S);

    // Setup matrix builder
    slater_uscf<double,double,double> slat(nbsf, nmo, nocca, noccb, S);
    wick_uscf<double,double,double> mb(orbs_a, orbs_b, S);
    slat.add_one_body(hcore);
    mb.add_one_body(hcore);

    // Variables for testing
    double swick = 0.0, fwick = 0.0, sslat = 0.0, fslat = 0.0;

    // Define "reference" occupation numbers
    arma::uvec ref_occa(nocca);
    arma::uvec ref_occb(noccb);
    for(size_t k=0; k<nocca; k++) ref_occa(k) = k;
    for(size_t k=0; k<noccb; k++) ref_occb(k) = k;

    // Excitation indices
    arma::umat xahp, xbhp;
    arma::umat wahp, wbhp;
    
    // Occupied orbitals
    arma::uvec xocca = ref_occa, xoccb = ref_occb;
    arma::uvec wocca = ref_occa, woccb = ref_occb;

    // Compute matrix elements
    mb.evaluate(xahp, xbhp, wahp, wbhp, swick, fwick);
    slat.evaluate(Cx_a.cols(xocca), Cx_b.cols(xoccb), Cw_a.cols(wocca), Cw_b.cols(woccb), sslat, fslat);

    size_t thresh = 8;
    // Test overlap
    if(std::abs(swick - sslat) > std::pow(0.1,thresh))
    {
        std::cout << "  Wick overlap = " << std::fixed << std::setw(thresh+10) 
                  << std::setprecision(thresh+1) << swick << std::endl
                  << "Slater overlap = " << std::fixed << std::setw(thresh+10)
                  << std::setprecision(thresh+1) << sslat << std::endl;
        return 1;
    }

    // Test one-body matrix element
    if(std::abs(fwick - fslat) > std::pow(0.1,thresh))
    {
        std::cout << "  Wick one-body = " << std::fixed << std::setw(thresh+10) 
                  << std::setprecision(thresh+1) << fwick << std::endl
                  << "Slater one-body = " << std::fixed << std::setw(thresh+10)
                  << std::setprecision(thresh+1) << fslat << std::endl;
        return 1;
    }

    std::cout << swick << std::endl;
    std::cout << fwick << std::endl;

    return 0;
}

