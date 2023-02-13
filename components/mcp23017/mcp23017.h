#pragma once

#include "esphome/core/component.h"
#include "esphome/core/hal.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome
{
    namespace mcp23017
    {
        // mcp23x17_base
        enum MCP23X17GPIORegisters
        {
            // A side
            MCP23X17_IODIRA = 0x00,
            MCP23X17_IPOLA = 0x02,
            MCP23X17_GPINTENA = 0x04,
            MCP23X17_DEFVALA = 0x06,
            MCP23X17_INTCONA = 0x08,
            MCP23X17_IOCONA = 0x0A,
            MCP23X17_GPPUA = 0x0C,
            MCP23X17_INTFA = 0x0E,
            MCP23X17_INTCAPA = 0x10,
            MCP23X17_GPIOA = 0x12,
            MCP23X17_OLATA = 0x14,
            // B side
            MCP23X17_IODIRB = 0x01,
            MCP23X17_IPOLB = 0x03,
            MCP23X17_GPINTENB = 0x05,
            MCP23X17_DEFVALB = 0x07,
            MCP23X17_INTCONB = 0x09,
            MCP23X17_IOCONB = 0x0B,
            MCP23X17_GPPUB = 0x0D,
            MCP23X17_INTFB = 0x0F,
            MCP23X17_INTCAPB = 0x11,
            MCP23X17_GPIOB = 0x13,
            MCP23X17_OLATB = 0x15,
        };

        // mcp23xxxbase
        enum MCP23XXXInterruptMode : uint8_t
        {
            MCP23XXX_NO_INTERRUPT = 0,
            MCP23XXX_CHANGE,
            MCP23XXX_RISING,
            MCP23XXX_FALLING
        };

        class MCP23017 : public i2c::I2CDevice
        {
        public:
            // mcp23017
            MCP23017() = default;

            void setup() override;
            void dump_config() override;

            // mcp23x17
            bool digital_read(uint8_t pin) override;
            void digital_write(uint8_t pin, bool value) override;
            void pin_mode(uint8_t pin, gpio::Flags flags) override;
            void pin_interrupt_mode(uint8_t pin, mcp23xxx_base::MCP23XXXInterruptMode interrupt_mode) override;

            // mcp23xxx
            virtual bool digital_read(uint8_t pin);
            virtual void digital_write(uint8_t pin, bool value);
            virtual void pin_mode(uint8_t pin, gpio::Flags flags);
            virtual void pin_interrupt_mode(uint8_t pin, MCP23XXXInterruptMode interrupt_mode);

            void set_open_drain_ints(const bool value) { this->open_drain_ints_ = value; }
            float get_setup_priority() const override;

        protected:
            // mcp23017
            bool read_reg(uint8_t reg, uint8_t *value) override;
            bool write_reg(uint8_t reg, uint8_t value) override;

            // mpc23x17
            void update_reg(uint8_t pin, bool pin_value, uint8_t reg_a) override;

            uint8_t olat_a_{0x00};
            uint8_t olat_b_{0x00};

            // mcp23xxx
            //  read a given register
            virtual bool read_reg(uint8_t reg, uint8_t *value);
            // write a value to a given register
            virtual bool write_reg(uint8_t reg, uint8_t value);
            // update registers with given pin value.
            virtual void update_reg(uint8_t pin, bool pin_value, uint8_t reg_a);

            bool open_drain_ints_;
        };

        class MCP23XXXGPIOPin : public GPIOPin
        {
        public:
            void setup() override;
            void pin_mode(gpio::Flags flags) override;
            bool digital_read() override;
            void digital_write(bool value) override;
            std::string dump_summary() const override;

            void set_parent(MCP23XXXBase *parent) { parent_ = parent; }
            void set_pin(uint8_t pin) { pin_ = pin; }
            void set_inverted(bool inverted) { inverted_ = inverted; }
            void set_flags(gpio::Flags flags) { flags_ = flags; }
            void set_interrupt_mode(MCP23XXXInterruptMode interrupt_mode) { interrupt_mode_ = interrupt_mode; }

        protected:
            MCP23XXXBase *parent_;
            uint8_t pin_;
            bool inverted_;
            gpio::Flags flags_;
            MCP23XXXInterruptMode interrupt_mode_;
        };

    } // namespace mcp23017
} // namespace esphome