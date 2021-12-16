#
#    Copyright (c) 2020 Project CHIP Authors
#    All rights reserved.
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#        http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.

#
# This is a project Makefile. It is assumed the directory this Makefile resides in is a
# project subdirectory.
#

include $(MAKE_INCLUDE_GEN)

.PHONY: all clean

#*****************************************************************************#
#                               VARIABLES	                              #
#*****************************************************************************#

DIR = $(SRCDIR)
DIR += $(CHIPDIR)/src/app
DIR += $(CHIPDIR)/src/app/server
DIR += $(CHIPDIR)/examples/lighting-app/lighting-common
DIR += $(CHIPDIR)/examples/lighting-app/ambd/main
DIR += $(CHIPDIR)/zzz_generated/lighting-app/zap-generated
DIR += $(CHIPDIR)/zzz_generated/app-common/app-common/zap-generated
DIR += $(CHIPDIR)/zzz_generated/app-common/app-common/zap-generated/attributes

vpath %.cpp $(DIR) $(shell find $(DIR) -type d)
vpath %.c $(DIR) $(shell find $(DIR) -type d)
#*****************************************************************************#
#                               CFLAGS                                        #
#*****************************************************************************#

GLOBAL_CFLAGS += -DCHIP_PROJECT=1

#*****************************************************************************#
#                               Include FILE LIST                             #
#*****************************************************************************#

#IFLAGS = -I$(DIR)./
IFLAGS += -I$(CHIPDIR)/zzz_generated/app-common
IFLAGS += -I$(CHIPDIR)/zzz_generated/lighting-app
IFLAGS += -I$(CHIPDIR)/zzz_generated/lighting-app/zap-generated
IFLAGS += -I$(CHIPDIR)/examples/lighting-app/lighting-common
IFLAGS += -I$(CHIPDIR)/examples/lighting-app/ambd/main/include
IFLAGS += -I$(CHIPDIR)/examples/lighting-app/ambd/build/chip/gen/include
IFLAGS += -I$(CHIPDIR)/src/include/
IFLAGS += -I$(CHIPDIR)/src/lib/
IFLAGS += -I$(CHIPDIR)/src/
IFLAGS += -I$(CHIPDIR)/third_party/nlassert/repo/include/
IFLAGS += -I$(CHIPDIR)/src/app/
IFLAGS += -I$(CHIPDIR)/src/app/util/
IFLAGS += -I$(CHIPDIR)/src/app/server/
IFLAGS += -I$(CHIPDIR)/src/controller/data_model
IFLAGS += -I$(CHIPDIR)/third_party/nlio/repo/include/
IFLAGS += -I$(CHIPDIR)/third_party/nlunit-test/repo/src

#*****************************************************************************#
#                               Source FILE LIST                              #
#*****************************************************************************#

#chip app
CPPSRC += $(CHIPDIR)/src/app/Command.cpp
CPPSRC += $(CHIPDIR)/src/app/CommandHandler.cpp
CPPSRC += $(CHIPDIR)/src/app/CommandSender.cpp
CPPSRC += $(CHIPDIR)/src/app/decoder.cpp
CPPSRC += $(CHIPDIR)/src/app/encoder-common.cpp
CPPSRC += $(CHIPDIR)/src/app/EventManagement.cpp
CPPSRC += $(CHIPDIR)/src/app/InteractionModelEngine.cpp
CPPSRC += $(CHIPDIR)/src/app/ReadClient.cpp
CPPSRC += $(CHIPDIR)/src/app/ReadHandler.cpp
CPPSRC += $(CHIPDIR)/src/app/WriteClient.cpp
CPPSRC += $(CHIPDIR)/src/app/WriteHandler.cpp

CPPSRC += $(CHIPDIR)/src/app/server/Server.cpp
CPPSRC += $(CHIPDIR)/src/app/server/Dnssd.cpp
CPPSRC += $(CHIPDIR)/src/app/server/EchoHandler.cpp
CPPSRC += $(CHIPDIR)/src/app/server/OnboardingCodesUtil.cpp
CPPSRC += $(CHIPDIR)/src/app/server/CommissioningWindowManager.cpp

CPPSRC += $(CHIPDIR)/src/app/util/af-event.cpp
CPPSRC += $(CHIPDIR)/src/app/util/af-main-common.cpp
CPPSRC += $(CHIPDIR)/src/app/util/attribute-storage.cpp
CPPSRC += $(CHIPDIR)/src/app/util/attribute-table.cpp
CPPSRC += $(CHIPDIR)/src/app/util/attribute-list-byte-span.cpp         #Base0518
CPPSRC += $(CHIPDIR)/src/app/util/attribute-size-util.cpp              #Base0617
CPPSRC += $(CHIPDIR)/src/app/util/binding-table.cpp
CPPSRC += $(CHIPDIR)/src/app/util/CHIPDeviceCallbacksMgr.cpp
CPPSRC += $(CHIPDIR)/src/app/util/chip-message-send.cpp
CPPSRC += $(CHIPDIR)/src/app/util/client-api.cpp
CPPSRC += $(CHIPDIR)/src/app/util/DataModelHandler.cpp
CPPSRC += $(CHIPDIR)/src/app/util/ember-compatibility-functions.cpp
CPPSRC += $(CHIPDIR)/src/app/util/ember-print.cpp
CPPSRC += $(CHIPDIR)/src/app/util/esi-management.cpp
CPPSRC += $(CHIPDIR)/src/app/util/message.cpp
CPPSRC += $(CHIPDIR)/src/app/util/process-cluster-message.cpp
CPPSRC += $(CHIPDIR)/src/app/util/process-global-message.cpp
CPPSRC += $(CHIPDIR)/src/app/util/util.cpp
CPPSRC += $(CHIPDIR)/src/app/util/error-mapping.cpp

CPPSRC += $(CHIPDIR)/src/app/clusters/basic/basic.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/bindings/bindings.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/diagnostic-logs-server/diagnostic-logs-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/general-commissioning-server/general-commissioning-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/on-off-server/on-off-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/operational-credentials-server/operational-credentials-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/level-control/level-control.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/network-commissioning/network-commissioning.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/network-commissioning/network-commissioning-ember.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/color-control-server/color-control-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/occupancy-sensor-server/occupancy-sensor-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/ethernet_network_diagnostics_server/ethernet_network_diagnostics_server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/software_diagnostics_server/software_diagnostics_server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/thread_network_diagnostics_server/thread_network_diagnostics_server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/wifi_network_diagnostics_server/wifi_network_diagnostics_server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/administrator-commissioning-server/administrator-commissioning-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/general_diagnostics_server/general_diagnostics_server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/mode-select-server/mode-select-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/mode-select-server/static-supported-modes-manager.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/groups-server/groups-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/identify-server/identify-server.cpp
CPPSRC += $(CHIPDIR)/src/app/clusters/scenes/scenes.cpp

#CPPSRC += $(CHIPDIR)/src/app/reporting/reporting-default-configuration.cpp
#CPPSRC += $(CHIPDIR)/src/app/reporting/reporting.cpp
CPPSRC += $(CHIPDIR)/src/app/reporting/Engine.cpp

CPPSRC += $(CHIPDIR)/zzz_generated/app-common/app-common/zap-generated/attributes/Accessors.cpp
CPPSRC += $(CHIPDIR)/zzz_generated/app-common/app-common/zap-generated/cluster-objects.cpp

CPPSRC += $(CHIPDIR)/zzz_generated/lighting-app/zap-generated/attribute-size.cpp
CPPSRC += $(CHIPDIR)/zzz_generated/lighting-app/zap-generated/callback-stub.cpp
CPPSRC += $(CHIPDIR)/zzz_generated/lighting-app/zap-generated/IMClusterCommandHandler.cpp

CPPSRC += $(CHIPDIR)/examples/lighting-app/ambd/main/chipinterface.cpp
CPPSRC += $(CHIPDIR)/examples/lighting-app/ambd/main/DeviceCallbacks.cpp
CPPSRC += $(CHIPDIR)/examples/lighting-app/ambd/main/CHIPDeviceManager.cpp
CPPSRC += $(CHIPDIR)/examples/lighting-app/ambd/main/Globals.cpp
CPPSRC += $(CHIPDIR)/examples/lighting-app/ambd/main/LEDWidget.cpp

#*****************************************************************************#
#                               Object FILE LIST                              #
#*****************************************************************************#
CPPOBJS = $(notdir $(CPPSRC:.cpp=.o))
COBJS = $(notdir $(CSRC:.c=.o))
OBJS = $(CPPOBJS)
OBJS += $(COBJS)
STATIC_LIB = lib_main.a

#*****************************************************************************#
#                               Include Dependency                            #
#*****************************************************************************#
-include $(OBJS:.o=.d)


#*****************************************************************************#
#                        RULES TO GENERATE TARGETS                            #
#*****************************************************************************#

# Define the Rules to build the core targets
all: CORE_TARGETS

#*****************************************************************************#
#              GENERATE OBJECT FILE
#*****************************************************************************#
#CORE_TARGETS:	$(CPPOBJS) $(COBJS)
CORE_TARGETS:	$(OBJS) $(STATIC_LIB)
$(STATIC_LIB):$(OBJS)
	$(AR) rvs $@ $^
	$(MOVE) -f $@ $(ROOTDIR)/lib/application

#*****************************************************************************#
#              CLEAN GENERATED FILES                                          #
#*****************************************************************************#
clean:
	rm -f $(ROOTDIR)/make/chip_main/lighting_app/*.o
	rm -f $(ROOTDIR)/make/chip_main/lighting_app/*.i
	rm -f $(ROOTDIR)/make/chip_main/lighting_app/*.s
	rm -f $(ROOTDIR)/make/chip_main/lighting_app/*.d
	rm -f $(ROOTDIR)/make/chip_main/lighting_app/*.ii
	rm -f $(ROOTDIR)/make/chip_main/lighting_app/*.su

-include $(DEPS)
