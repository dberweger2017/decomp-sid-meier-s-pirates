#pragma once

namespace Phono2 {

// The observed constructor initializes only these first two words.
class PThread {
public:
    PThread();

private:
    unsigned int m_state;
    unsigned int m_defaultValue;
};

} // namespace Phono2
