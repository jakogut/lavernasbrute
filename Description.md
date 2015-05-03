# Introduction #

Laverna's Brute is a free and open source platform for attacking or reversing cryptographic algorithms, ciphers, and hashes. The main goal of the project is to provide the fastest, easiest, cheapest, most extensible, and flexible tool for this job. The final and most important milestone for the Laverna project is to create a distributed cracking network powered by its users.

Essentially, each user will provide a hash, or list of hashes, that they want cracked. In order to receive the reversed hashes, they will need to donate processing time to the project, during which they will help crack hashes submitted by other users. Due to the highly parallel nature of attacking hashes, adding target hashes to the queue affects the network's performance very little. The processing time requirement ensures that each client is donating many times more to the network than they take.

# Legal #

Laverna's Brute is, quite simply, a tool. The purpose of this tool is to break or reverse cryptographic ciphers, hashes, and algorithms. It can be used to reveal the vulnerability of a system for the purpose of improving the security of that system, and it can be used to break into a system unlawfully. The tool itself is neutral, and its purpose, whether for good or evil, depends solely on the intent of the user. I do not condone the misuse of this tool, and I strongly advise that you check up on your local laws and regulations before using Laverna's Brute. Only use it on equipment that you own, or are otherwise authorized to use. Failure to abide by your local laws and regulations when using Laverna's Brute is a misuse of the tool, and I am not responsible for any penalties or consequences you may encounter as a result.

# Milestones: #

  * Multithreading **Complete**

  * Multihashing **Complete**

  * Supported platforms:
    * Windows **Working, but not officially supported**
    * Linux 2.6 **Complete**
    * Mac OS 10.x **Complete**
    * FreeBSD **Untested, but should work**
    * NetBSD **Untested, but should work**

  * Support for hashing/cryptographic algorithms:
    * NTLM (Windows hashes) **Complete**
    * MD4 **Partial**
    * MD5 **Not started**
    * Blowfish **Not started**

  * SSE/SSE2 acceleration
    * NTLM **Complete**
    * MD4 **Partial**
    * MD5 **Not started**
    * Blowfish **Not started**

  * MMX acceleration
    * NTLM **Partial**
    * MD4 **Partial**
    * MD5 **Not started**
    * Blowfish **Not started**

  * Alternative Processing Paths:
    * ATI (Stream) processing path **Not started**
    * NVIDIA (CUDA) processing path **Not started**
    * OpenCL processing path **Not started**
    * Networked load distribution/coordination **Not started**

  * Persistent runs (Saving/loading of progress in a run) **Not started**
> > Attacks running on a single local machine should catch the interrupt, and save
> > their progress to a database before exiting. This will allow for automatic
> > resuming when the same hash is attacked later.

# Milestone Release Plan #

Version 1.0 **Released**

> Cracker is stable, and is capable of attacking some form of hash. Shows at least
> moderate amount of usefulness.

Version 2.0
> Cracker has been adapted to attack hashes using a graphical processing unit (GPU)

Version 3.0
> Some form of networking support has been implemented, and clients can communicate and
> coordinate a distributed attack over a local area network.

Version 4.0
> Unplanned.

# Release Labels: #

## Development Releases: ##
> ### Alpha ###

  * Most unstable form of development release

  * Feature list is not frozen, and new bugs/regressions may be introduced at any time

  * Intended feature list is not complete

> ### Beta ###

  * A step up in stability from Alpha

  * Feature list for a given release is, for the most part, completed

  * Code changes are primarily bug fixes

> ### Release Candidate ###

  * RC builds have potential to be release builds, but are still potentially unstable

  * Feature list is frozen, no new features may be introduced in this stage

  * Bug fixes only

## Release Builds: ##

Once a certain release has passed the RC stage, and has been deemed stable, it will be compiled and binaries for a number of platforms will be hosted here.