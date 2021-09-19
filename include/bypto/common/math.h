#pragma once

namespace bypto::common::math {

    template <typename X,typename Y>
    Y interpolate(X x1,Y y1,X x2,Y y2, X x3) {
        return (y2 - y1) / (x2 - x1) * (x3 - x1) + y1;
    }

}