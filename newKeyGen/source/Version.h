#ifndef VERSION_H_
#define VERSION_H_

#define VERSION "Development r351"
// #define VERSION "Release 1.1"

#define COPYRIGHT "2008-2010"

/* Enable functions which aid in developing and debugging for Laverna's Brute.
Comment this out for release builds. */
// #define DEBUG

/* Include support for SSE.
If the processor doesn't support SSE, this can be disabled for compatibility.
This should not be necessary, as any processor not supporting SSE is not worth $
to the Laverna project, in terms of processing power. */
#define SSE

#endif
