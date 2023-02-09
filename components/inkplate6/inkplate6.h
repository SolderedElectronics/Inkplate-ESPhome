#pragma once

#include "esphome/core/component.h"

namespace esphome
{
    namespace inkplate6
    {

        class Inkplate6 : public Component
        {
            public:
                void setup() override;
                void update() override;
                void test();
        }

    } // namespace inkplate6
} // namespace esphome
