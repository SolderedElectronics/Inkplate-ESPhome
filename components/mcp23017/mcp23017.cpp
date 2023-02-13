#include "mcp23017.h"

namespace esphome
{
    namespace mcp23017
    {
        static const char *const TAG = "mcp23017";

        // mcp23xxx
        float MCP23XXXBase::get_setup_priority() const { return setup_priority::IO; }

        void MCP23017::setup() { pin_mode(flags_); }
        void MCP23017::pin_mode(gpio::Flags flags) { this->parent_->pin_mode(this->pin_, flags); }
        bool MCP23017::digital_read() { return this->parent_->digital_read(this->pin_) != this->inverted_; }
        void MCP23017::digital_write(bool value) { this->parent_->digital_write(this->pin_, value != this->inverted_); }
        std::string MCP23017::dump_summary() const
        {
            char buffer[32];
            snprintf(buffer, sizeof(buffer), "%u via MCP23XXX", pin_);
            return buffer;
        }

        // mcp23x17
        bool MCP23017::digital_read(uint8_t pin)
        {
            uint8_t bit = pin % 8;
            uint8_t reg_addr = pin < 8 ? MCP23X17_GPIOA : MCP23X17_GPIOB;
            uint8_t value = 0;
            this->read_reg(reg_addr, &value);
            return value & (1 << bit);
        }

        void MCP23017::digital_write(uint8_t pin, bool value)
        {
            uint8_t reg_addr = pin < 8 ? MCP23X17_OLATA : MCP23X17_OLATB;
            this->update_reg(pin, value, reg_addr);
        }

        void MCP23017::pin_mode(uint8_t pin, gpio::Flags flags)
        {
            uint8_t iodir = pin < 8 ? MCP23X17_IODIRA : MCP23X17_IODIRB;
            uint8_t gppu = pin < 8 ? MCP23X17_GPPUA : MCP23X17_GPPUB;
            if (flags == gpio::FLAG_INPUT)
            {
                this->update_reg(pin, true, iodir);
                this->update_reg(pin, false, gppu);
            }
            else if (flags == (gpio::FLAG_INPUT | gpio::FLAG_PULLUP))
            {
                this->update_reg(pin, true, iodir);
                this->update_reg(pin, true, gppu);
            }
            else if (flags == gpio::FLAG_OUTPUT)
            {
                this->update_reg(pin, false, iodir);
            }
        }

        void MCP23017::pin_interrupt_mode(uint8_t pin, MCP23XXXInterruptMode interrupt_mode)
        {
            uint8_t gpinten = pin < 8 ? MCP23X17_GPINTENA : MCP23X17_GPINTENB;
            uint8_t intcon = pin < 8 ? MCP23X17_INTCONA : MCP23X17_INTCONB;
            uint8_t defval = pin < 8 ? MCP23X17_DEFVALA : MCP23X17_DEFVALB;

            switch (interrupt_mode)
            {
            case MCP23XXX_CHANGE:
                this->update_reg(pin, true, gpinten);
                this->update_reg(pin, false, intcon);
                break;
            case MCP23XXX_RISING:
                this->update_reg(pin, true, gpinten);
                this->update_reg(pin, true, intcon);
                this->update_reg(pin, true, defval);
                break;
            case MCP23XXX_FALLING:
                this->update_reg(pin, true, gpinten);
                this->update_reg(pin, true, intcon);
                this->update_reg(pin, false, defval);
                break;
            case MCP23XXX_NO_INTERRUPT:
                this->update_reg(pin, false, gpinten);
                break;
            }
        }

        void MCP23017::update_reg(uint8_t pin, bool pin_value, uint8_t reg_addr)
        {
            uint8_t bit = pin % 8;
            uint8_t reg_value = 0;
            if (reg_addr == MCP23X17_OLATA)
            {
                reg_value = this->olat_a_;
            }
            else if (reg_addr == MCP23X17_OLATB)
            {
                reg_value = this->olat_b_;
            }
            else
            {
                this->read_reg(reg_addr, &reg_value);
            }

            if (pin_value)
            {
                reg_value |= 1 << bit;
            }
            else
            {
                reg_value &= ~(1 << bit);
            }

            this->write_reg(reg_addr, reg_value);

            if (reg_addr == MCP23X17_OLATA)
            {
                this->olat_a_ = reg_value;
            }
            else if (reg_addr == MCP23X17_OLATB)
            {
                this->olat_b_ = reg_value;
            }
        }

        // mcp23017
        void MCP23017::setup()
        {
            ESP_LOGCONFIG(TAG, "Setting up MCP23017...");
            uint8_t iocon;
            if (!this->read_reg(MCP23X17_IOCONA, &iocon))
            {
                this->mark_failed();
                return;
            }

            // Read current output register state
            this->read_reg(MCP23X17_OLATA, &this->olat_a_);
            this->read_reg(MCP23X17_OLATB, &this->olat_b_);

            if (this->open_drain_ints_)
            {
                // enable open-drain interrupt pins, 3.3V-safe
                this->write_reg(MCP23X17_IOCONA, 0x04);
                this->write_reg(MCP23X17_IOCONB, 0x04);
            }
        }

        void MCP23017::dump_config() { ESP_LOGCONFIG(TAG, "MCP23017:"); }

        bool MCP23017::read_reg(uint8_t reg, uint8_t *value)
        {
            if (this->is_failed())
                return false;

            return this->read_byte(reg, value);
        }
        bool MCP23017::write_reg(uint8_t reg, uint8_t value)
        {
            if (this->is_failed())
                return false;

            return this->write_byte(reg, value);
        }

    } // namespace mcp23017
} // namespace esphome