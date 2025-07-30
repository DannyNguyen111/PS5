// Copyright 2025 Danny Nguyen
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE RandWriterTests

#include <boost/test/unit_test.hpp>
#include "RandWriter.hpp"

// Test freq(kgram) with order-2 model
BOOST_AUTO_TEST_CASE(FreqTest) {
    std::string text = "gagggagaggcgagaaa";
    RandWriter rw(text, 2);

    BOOST_REQUIRE_EQUAL(rw.freq("ga"), 5);   // e.g. "ga" appears 5 times
    BOOST_REQUIRE_EQUAL(rw.freq("gg"), 3);   // e.g. "gg" appears 3 times
    BOOST_REQUIRE_THROW(rw.freq("g"), std::invalid_argument);  // wrong length
    BOOST_REQUIRE_EQUAL(rw.freq("zz"), 0);   // never appears
}

// Test freq(kgram, char) with order-2 model
BOOST_AUTO_TEST_CASE(FreqCharTest) {
    std::string text = "gagggagaggcgagaaa";
    RandWriter rw(text, 2);

    BOOST_REQUIRE_EQUAL(rw.freq("ga", 'g'), 4);  // freq of "ga" followed by 'g'
    BOOST_REQUIRE_EQUAL(rw.freq("ga", 'z'), 0);  // freq of "ga" followed by 'z' is zero
    BOOST_REQUIRE_THROW(rw.freq("g", 'a'), std::invalid_argument);  // wrong length
}

// Test kRand returns a valid char
BOOST_AUTO_TEST_CASE(KRandTest) {
    std::string text = "gagggagaggcgagaaa";
    RandWriter rw(text, 2);

    BOOST_REQUIRE_NO_THROW(rw.kRand("ga"));     // Should not throw
    char c = rw.kRand("ga");
    BOOST_REQUIRE_NE(c, '\0');                  // Should produce a char
}

// Test generate returns correct length and begins with kgram
BOOST_AUTO_TEST_CASE(GenerateTest) {
    std::string text = "gagggagaggcgagaaa";
    RandWriter rw(text, 2);

    std::string result = rw.generate("ga", 10);  // length 10
    BOOST_REQUIRE_EQUAL(result.size(), 10);     // Check length
    BOOST_REQUIRE_EQUAL(result.substr(0, 2), "ga");  // Check seed
    BOOST_REQUIRE_THROW(rw.generate("g", 10), std::invalid_argument);  // kgram too short
    BOOST_REQUIRE_THROW(rw.generate("ga", 1), std::invalid_argument);  // L < k
}

