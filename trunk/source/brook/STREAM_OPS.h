#ifndef _STREAM_OPS_AUTO_GENERATED_H_
#define _STREAM_OPS_AUTO_GENERATED_H_

/**************************************************************************** 
                                                                              
Copyright (c) 2003, Stanford University                                       
All rights reserved.                                                          
                                                                              
Copyright (c) 2008, Advanced Micro Devices, Inc.                              
All rights reserved.                                                          
                                                                              
                                                                              
The BRCC portion of BrookGPU is derived from the cTool project                
(http://ctool.sourceforge.net) and distributed under the GNU Public License.  
                                                                              
Additionally, see LICENSE.ctool for information on redistributing the         
contents of this directory.                                                   
                                                                              
****************************************************************************/ 

#include "brook/Stream.h" 
#include "brook/KernelInterface.h" 

//! Kernel declarations
class __gpu_ADD
{
    public:
        void operator()(const ::brook::Stream< float >& input0, const ::brook::Stream< float >& input1, const ::brook::Stream<  float >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_ADD gpu_ADD;

class __gpu_SUB
{
    public:
        void operator()(const ::brook::Stream< float >& input0, const ::brook::Stream< float >& input1, const ::brook::Stream<  float >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_SUB gpu_SUB;

class __gpu_DIV
{
    public:
        void operator()(const ::brook::Stream< float >& input0, const ::brook::Stream< float >& input1, const ::brook::Stream<  float >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_DIV gpu_DIV;

class __gpu_MUL
{
    public:
        void operator()(const ::brook::Stream< float >& input0, const ::brook::Stream< float >& input1, const ::brook::Stream<  float >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_MUL gpu_MUL;

class __gpu_NOT
{
    public:
        void operator()(const ::brook::Stream< int >& input, const ::brook::Stream<  int >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_NOT gpu_NOT;

class __gpu_OR
{
    public:
        void operator()(const ::brook::Stream< int >& input0, const ::brook::Stream< int >& input1, const ::brook::Stream<  int >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_OR gpu_OR;

class __gpu_XOR
{
    public:
        void operator()(const ::brook::Stream< int >& input0, const ::brook::Stream< int >& input1, const ::brook::Stream<  int >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_XOR gpu_XOR;

class __gpu_AND
{
    public:
        void operator()(const ::brook::Stream< int >& input0, const ::brook::Stream< int >& input1, const ::brook::Stream<  int >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_AND gpu_AND;

class __gpu_LSHIFT
{
    public:
        void operator()(const ::brook::Stream< int >& input, const int  shift, const ::brook::Stream<  int >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_LSHIFT gpu_LSHIFT;

class __gpu_RSHIFT
{
    public:
        void operator()(const ::brook::Stream< int >& input, const int  shift, const ::brook::Stream<  int >& output);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_RSHIFT gpu_RSHIFT;

#endif // _STREAM_OPS_AUTO_GENERATED_H_

