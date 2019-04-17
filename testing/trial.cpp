#include "catch.hpp"
#include <fitter_module.h>
#include <vector>


std::vector<GeigerRing> lineA() {
  // hard-coded image data provider
  // positive x, flat line, fixed radius
  double xset[9] = {53.0, 97.0, 141.0, 185.0, 229.0, 273.0, 317.0, 361.0, 405.0};
  GeigerRing ring;
  std::vector<GeigerRing> rings;
  for (unsigned int j=0;j<9;j++) {
    ring.rerr   = 0.9;
    ring.zerr = 10.0;
    ring.radius = 10.0;
    ring.wirex  = xset[j];
    ring.wirey  = 15.5;
    ring.zcoord = 1.0;
    rings.push_back(ring);
  }

  return rings;
}


std::vector<GeigerRing> lineB() {
  // hard-coded image data provider
  // negative x, flat line, fixed radius
  double xset[9] = {-53.0, -97.0, -141.0, -185.0, -229.0, -273.0, -317.0, -361.0, -405.0};
  GeigerRing ring;
  std::vector<GeigerRing> rings;
  for (unsigned int j=0;j<9;j++) {
    ring.rerr   = 0.9;
    ring.zerr = 11.0;
    ring.radius = 16.0;
    ring.wirex  = xset[j];
    ring.wirey  = 15.5;
    ring.zcoord = 1.0;
    rings.push_back(ring);
  }

  return rings;
}


std::vector<GeigerRing> lineC() {
  // hard-coded image data provider
  // positive x, slope sign ambiguity line
  double xset[9] = {53.0, 97.0, 141.0, 185.0, 229.0, 273.0, 317.0, 361.0, 405.0};
  double rset[9] = {18.1303, 15.5314, 12.9326, 10.3337, 7.73486, 5.136, 2.53715, 0.06171, 2.66057};
  GeigerRing ring;
  std::vector<GeigerRing> rings;
  for (unsigned int j=0;j<9;j++) {
    ring.rerr   = 0.9;
    ring.zerr = 10.0;
    ring.radius = rset[j];
    ring.wirex  = xset[j];
    ring.wirey  = -352.0;
    ring.zcoord = 1.0;
    rings.push_back(ring);
  }

  return rings;
}


int check_lineA(){
  SNFitter snf;
  std::vector<GeigerRing> rings = lineA();
  int fails = 0;
  std::vector<LineFit> res = snf.fitline(rings);
  for (LineFit lf : res) { // should contain 4 candidates
    if (lf.status>0) // 4 chances to fail
      fails++;
  }
  return fails;
}

bool check_lineB(){
  SNFitter snf;
  bool found = false;
  std::vector<GeigerRing> rings = lineB();
  std::vector<LineFit> res = snf.fitline(rings);
  for (LineFit lf : res) { // should contain 4 candidates
    if (lf.ixy>31.0 && lf.ixy<32.0) // for one: near 31.5 with small error
      found = true;
  }
  return found;
}


bool check_lineC1(){
  SNFitter snf;
  bool found = false;
  std::vector<GeigerRing> rings = lineC();
  std::vector<LineFit> res = snf.fitline(rings);
  for (LineFit lf : res) { // should contain 4 candidates
    if (lf.slxy>0.0591 && lf.slxy<0.0592) // for two cases: near 0.0591x
      found = true;
  }
  return found;
}


bool check_lineC2(){
  SNFitter snf;
  bool large = false;
  std::vector<GeigerRing> rings = lineC();
  std::vector<LineFit> res = snf.fitline(rings);
  for (LineFit lf : res) { // should contain 4 candidates
    if (lf.chi2 >= 1.0e-6) // for all: less than 1.0e-6 in tests
      large = true;
  }
  return large;
}




TEST_CASE( "Line A", "[falaise][fitstatus]" ) {
  REQUIRE( check_lineA() == 0 );
}

TEST_CASE( "Line B", "[falaise][fitcheck]" ) {
  REQUIRE( check_lineB() == true );
}

TEST_CASE( "Line C1", "[falaise][fitterslopes]" ) {
  REQUIRE( check_lineC1() == true );
}

TEST_CASE( "Line C2", "[falaise][fitterchi2]" ) {
  REQUIRE( check_lineC2() == false );
}

