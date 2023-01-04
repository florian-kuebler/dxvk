// Copyright (c) 2023 The Orbit Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ORBIT_API_INTERFACE_ORBIT_DXVK_H_
#define ORBIT_API_INTERFACE_ORBIT_DXVK_H_

#include "OrbitApiInterface/Orbit.h"
#include <atomic>
#include <string>

// This file contains a set of helper macros for Orbit's
// manual instrumentation in the context of DXVK.

// Used to group together Dx calls and their emitted callbacks.
extern std::atomic<uint64_t> kOrbitGlobalGroupId;

#if ORBIT_CPU_INSTRUMENTATION
// Computes the next unique global group id (`kOrbitGlobalGroupId`),
// sets `group_id_ptr` and creates an `ORBIT_SCOPE` with that id.
// As name it will use the function's name with the prefix "DXVK__".
#define ORBIT_SCOPE_DXVK_FUNCTION_WITH_NEXT_GROUP_ID(group_id_ptr) \
  *group_id_ptr = kOrbitGlobalGroupId++;                      \
  ORBIT_SCOPE_WITH_GROUP_ID(                                       \
    std::string("DXVK__").append(__FUNCTION__).c_str(),            \
    *group_id_ptr)                                                 \

#define ORBIT_SCOPE_DXVK_FUNCTION() \
  ORBIT_SCOPE(std::string("DXVK__").append(__FUNCTION__).c_str())

#define ORBIT_SCOPE_DXVK_WITH_GROUP_ID(label, group_id) \
  ORBIT_SCOPE_WITH_GROUP_ID(label, group_id)

#define ORBIT_SCOPE_VK(vkFunction) ORBIT_SCOPE(vkFunction)

#define ORBIT_SCOPE_VK_WITH_NEXT_GROUP_ID(vkFunction, groupIdPtr)          \
  *groupIdPtr = kOrbitGlobalGroupId++;                                \
  ORBIT_SCOPE_WITH_GROUP_ID(vkFunction, *groupIdPtr)

#else
#define ORBIT_SCOPE_DXVK_FUNCTION_WITH_NEXT_GROUP_ID(group_id_ptr) \
  (void)group_id_ptr

#define ORBIT_SCOPE_DXVK_FUNCTION()

#define ORBIT_SCOPE_DXVK_WITH_GROUP_ID(label, group_id) \
  (void)group_id

#define ORBIT_SCOPE_VK(vkFunction)

#define ORBIT_SCOPE_VK_WITH_NEXT_GROUP_ID(vkFunction, groupIdPtr) \
  (void)groupIdPtr
#endif

#if ORBIT_GPU_INSTRUMENTATION
#define ORBIT_VK_BEGIN_DEBUG_LABEL(vkFunction, commandBuffer, groupIdPtr)       \
  do {                                                                          \
    if (m_vkd->vkCmdBeginDebugUtilsLabelEXT != nullptr) {                       \
      const std::string cLabelName =                                            \
        std::string(vkFunction).append("#").append(std::to_string(*groupIdPtr));\
      const VkDebugUtilsLabelEXT cLabel{                                        \
        VK_STRUCTURE_TYPE_DEBUG_UTILS_LABEL_EXT,                                \
        nullptr,                                                                \
        cLabelName.c_str()};                                                    \
        m_vkd->vkCmdBeginDebugUtilsLabelEXT(commandBuffer, &cLabel);            \
    }                                                                           \
  } while (false)

#define ORBIT_VK_END_DEBUG_LABEL(commandBuffer)          \
  do {                                                   \
    if (m_vkd->vkCmdEndDebugUtilsLabelEXT != nullptr)    \
       m_vkd->vkCmdEndDebugUtilsLabelEXT(commandBuffer); \
  } while (false)
#else  // ORBIT_GPU_INSTRUMENTATION
#define ORBIT_VK_BEGIN_DEBUG_LABEL(vkFunction, commandBuffer, groupIdPtr) \
  (void)groupIdPtr

#define ORBIT_VK_END_DEBUG_LABEL(commandBuffer)

#endif

#endif  // ORBIT_API_INTERFACE_ORBIT_DXVK_H_
