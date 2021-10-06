#pragma once

namespace bypto::strategy::decision {

    enum Choice { Buy, Sell, Nothing };

    template <Choice c>
    struct Decision {
        Choice get_decision() {
            return c;
        }
    };
}