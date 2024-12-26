#include "debug_print.h"

NullStream SelectStream<false>::stream_object_;

std::conditional<DebugPrint::enable_print,std::ostream,NullStream>::type &
DebugPrint::stream = SelectStream<DebugPrint::enable_print>::stream_object_;

//
//END-OF-FILE
//
