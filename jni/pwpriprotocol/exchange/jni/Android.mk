# Copyright (C) 2009 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_CPPFLAGS+= -fexceptions
LOCAL_MODULE    := libtplayer

LOCAL_SRC_FILES :=../Source/CameraExchange.cpp\
		../Source/CaptureExchange.cpp\
		../Source/ChainExchange.cpp\
		../Source/CommExchange.cpp\
		../Source/Exchange.cpp\
		../Source/ExchangeKind.cpp\
		../Source/GeneralExchange.cpp\
		../Source/GUIExchange.cpp\
		../Source/ManagerExchange.cpp\
		../Source/MediaExchange.cpp\
		../Source/NetIPAbilitySet.cpp\
		../Source/NetIPDeviceInfo.cpp\
		../Source/NetIPOperation.cpp\
		../Source/NetPlatform.cpp\
		../Source/NetWorkExchange.cpp\
		../Source/StorageExchange.cpp\	


include $(BUILD_STATIC_LIBRARY)
$(call import-module,android/support)