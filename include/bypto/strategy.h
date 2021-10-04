#pragma once

#include "bypto/data/price.h"
namespace bypto::strategy {

    enum Indicator { MovingAverage };

    template <Indicator S,data::price::PriceSource P>
    class Strategy;

}