#ifndef SEMESTRALKAMAKE_TESTS_H
#pragma once
#include "load.h"

/**
 * Unit tests for correct config format
 */
void loadTest(){
    CData testData;
    assert(!CLoader("./assets/configTest/config1.json").parse(testData));
    assert(!CLoader("./assets/configTest/config2.json").parse(testData));
    assert(!CLoader("./assets/configTest/config3.json").parse(testData));
    assert(!CLoader("./assets/configTest/config4.json").parse(testData));
    assert(!CLoader("./assets/configTest/config5.json").parse(testData));
    assert(!CLoader("./assets/configTest/config8.json").parse(testData));
    assert(!CLoader("./assets/configTest/config9.json").parse(testData));
    assert(!CLoader("./assets/configTest/config10.json").parse(testData));
    assert(!CLoader("./assets/configTest/config11.json").parse(testData));
    assert(!CLoader("./assets/configTest/config12.json").parse(testData));
    assert(!CLoader("./assets/configTest/config13.json").parse(testData));
    assert(!CLoader("./assets/configTest/config14.json").parse(testData));
    assert(!CLoader("./assets/configTest/config15.json").parse(testData));
    assert(!CLoader("./assets/configTest/config16.json").parse(testData));
    assert(!CLoader("./assets/configTest/config17.json").parse(testData));
    assert(!CLoader("./assets/configTest/config18.json").parse(testData));
    assert(!CLoader("./assets/configTest/config19.json").parse(testData));
    assert(!CLoader("./assets/configTest/config20.json").parse(testData));
    assert(!CLoader("./assets/configTest/dummy.png").parse(testData));
    assert(!CLoader("wrongFilePath").parse(testData));
}

#define SEMESTRALKAMAKE_TESTS_H

#endif //SEMESTRALKAMAKE_TESTS_H
