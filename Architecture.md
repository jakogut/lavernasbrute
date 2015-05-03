# Introduction #

Laverna's Brute is an advanced hash-cracking application built on top of a massively parallel computational engine. The engine provides easy access to a number of processing paths, and structures it in a way that is easy to understand and extend.

# Details #

## Laverna's Brute ##

The code specific to Laverna's Brute is specialized in hash-cracking. The real power of the cracker lies in the processing engine powering it.

  * ## Master Thread ##
Master thread is a class used to create a single thread which monitors the operation of worker threads, and performs tasks for the whole of the program, such as writing information to the console, tracking the progress and success of a run, and file I/O. This structure eliminates the need for the usage of semaphores.

  * ## CPU Path ##
The CPU path is a class derived from the processing path class. It is used to create worker threads which depend on the power of the CPU for operation. This path is thread-safe.

  * ## GPU Path ##
The GPU path will be a class derived from the processing path class. It will be used to create worker threads which depend on the power of the GPU for operation.

  * ## NTLM ##
The NTLM class is used to provide NTLM hashing of text strings.


## Processing Engine ##

The Laverna's Brute processing engine utilizes a number of methods for parallel computation on a single machine, such as threading and SIMD operations, which are meant to use as much power available, as efficiently as possible. The engine is planned to be extended with support for organized distributed computing over a network.

  * ## Processing Path ##
The processing path class is a base class containing objects common to all paths, which implementations can derive from.

  * ## Director ##
The Director is a thread object which is used to track and manage information from various threads. Its current task is solely as a sort of work manager that makes work for idle processing paths when necessary. In the future, however, it can be extended to consolidate and save the progress of individual processing paths to create a persistent run system.

The director can manage any thread derived from the processing path class that has defined the appropriate functions used modify the thread's local keyspace.