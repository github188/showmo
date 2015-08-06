include config

TOPDIR = ./

CFLAGS	+= -I$(TOPDIR)Include
CFLAGS  += -Wall -O2 -Os 
CFLAGS  += -D_GNU_SOURCE -DLINUX
CFLAGS  += -fomit-frame-pointer
#CFLAGS  += -Wstrict-prototypes

AFLAGS	+= -rv

LIB_TARGET = $(LIBDIR)libExchanger.a

LIB_OBJS = Source/CameraExchange.o	\
		Source/CaptureExchange.o	\
		Source/ChainExchange.o		\
		Source/CommExchange.o		\
		Source/Exchange.o			\
		Source/ExchangeKind.o		\
		Source/GeneralExchange.o	\
		Source/GUIExchange.o		\
		Source/ManagerExchange.o	\
		Source/MediaExchange.o		\
		Source/NetIPAbilitySet.o	\
		Source/NetIPDeviceInfo.o	\
		Source/NetIPOperation.o		\
		Source/NetPlatform.o		\
		Source/NetWorkExchange.o	\
		Source/StorageExchange.o	

TEST_OBJS = Test/Main.o	

TEST_TARGET = $(BINDIR)/NetIP

TARGET	= $(LIB_TARGET) #$(TEST_TARGET)

all: $(TARGET)
	
$(LIB_TARGET): $(LIB_OBJS)
	$(RM) $@
	$(AR) $(AFLAGS) $@ $^
#	cp $@ $(ALLOCA_DIR)/$(PLATFORM)/Package/
#	cp $@ $(Package_LIBS)/$(PLATFORM)/
	@echo "#################################################################################"
	@echo $(CFLAGS)
	@echo "#################################################################################" 

$(TEST_TARGET): $(TEST_OBJS) $(LIB_TARGET) $(EXTLIBS)
	$(CPP) -static -o $@ $^ $(LDFLAGS)

.c.o:
	$(CC) -c $(CFLAGS) $^ -o $@

.cpp.o:
	$(CPP) -c $(CFLAGS) $^ -o $@

clean:
	$(RM) $(TARGET) $(LIB_OBJS) $(TEST_OBJS);

