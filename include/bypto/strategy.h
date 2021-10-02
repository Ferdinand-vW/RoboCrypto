#pragma once

namespace bypto::strategy {

    enum Strats { MovingAverage };

    template <Strats S>
    class Strategy;

}