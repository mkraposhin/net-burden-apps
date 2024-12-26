#ifndef DEBUG_PRINT_H
#define DEBUG_PRINT_H

#include <iostream>

/// The class represents a NULL output stream which 
///doesn't output spicified data anywhere
struct NullStream
{
};
template <class T>
NullStream& operator << (NullStream & null_stream, const T& val)
{
    return null_stream;
}

template<bool debug_enabled>
struct SelectStream
{
    static std::ostream &stream_object_;
};
template<bool debug_enabled>
std::ostream &SelectStream<debug_enabled>::stream_object_ = std::cout;

template<>
struct SelectStream<false>
{
    static NullStream stream_object_;
};

/// The class is used to switch and manage debug print
struct DebugPrint
{
    /// Enables or disables debug printing
    static constexpr bool enable_print = true;

    /// Sets type of output stream (null object when debug printing is disabled)
    /// and std::cout when debug printing is enabled
    static std::conditional<enable_print,std::ostream,NullStream>::type &
        stream;
};

#endif
//
//END-OF-FILE
//
