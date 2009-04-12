#ifndef _NTLM_GPU_AUTO_GENERATED_H_
#define _NTLM_GPU_AUTO_GENERATED_H_

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
class __gpu_ntlm_crypt
{
    public:
        void operator()(const ::brook::Stream< uint >& nt_buffer_0, const ::brook::Stream< uint >& nt_buffer_1, const ::brook::Stream< uint >& nt_buffer_2, const ::brook::Stream< uint >& nt_buffer_3, const ::brook::Stream< uint >& nt_buffer_4, const ::brook::Stream< uint >& nt_buffer_5, const ::brook::Stream< uint >& nt_buffer_6, const ::brook::Stream< uint >& nt_buffer_7, const ::brook::Stream< uint >& nt_buffer_8, const ::brook::Stream< uint >& nt_buffer_9, const ::brook::Stream< uint >& nt_buffer_10, const ::brook::Stream< uint >& nt_buffer_11, const ::brook::Stream< uint >& nt_buffer_12, const ::brook::Stream< uint >& nt_buffer_13, const ::brook::Stream< uint >& nt_buffer_14, const ::brook::Stream< uint >& nt_buffer_15, const ::brook::Stream<  uint >& output_0, const ::brook::Stream<  uint >& output_1, const ::brook::Stream<  uint >& output_2, const ::brook::Stream<  uint >& output_3);
        EXTENDCLASS();
};
extern __THREAD__ __gpu_ntlm_crypt gpu_ntlm_crypt;

#endif // _NTLM_GPU_AUTO_GENERATED_H_

