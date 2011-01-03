CC  = arm-gph-linux-gnueabi-gcc.exe
CXX = arm-gph-linux-gnueabi-g++.exe
DEFS = -DGP2X -DCAANOO -DLSB_FIRST=1 -D__NO_STRING_INLINES -DHAVE_ASPRINTF
CFLAGS += -Wall -Wno-write-strings -Wno-sign-compare -O5 -mcpu=arm926ej-s -mtune=arm926ej-s
CFLAGS += -fsigned-char -ffast-math -fomit-frame-pointer -fexpensive-optimizations -fno-strength-reduce 
CAANOODEV = /cygdrive/c/Program Files (x86)/CAANOO/GPH_SDK
LFLAGS = -L'$(CAANOODEV)/target/lib' -I'$(CAANOODEV)/include -DGP2X' -lz
TARGET = fc2x.gpe
STRIP = arm-gph-linux-gnueabi-strip.exe
SRC = ./source/
SRCUTILS = $(SRC)utils/
SRCBOARDS = $(SRC)boards/
SRCINPUT = $(SRC)input/
SRCMAPPER = $(SRC)mappers/
CAANOO_DRV = $(SRC)drivers/caanoo/
COM_DRV = $(SRC)drivers/common/

OBJS = $(SRCBOARDS)01-222.o $(SRCBOARDS)3d-block.o $(SRCBOARDS)15.o \
$(SRCBOARDS)23.o $(SRCBOARDS)43.o $(SRCBOARDS)57.o $(SRCBOARDS)68.o $(SRCBOARDS)88.o $(SRCBOARDS)90.o \
$(SRCBOARDS)95.o $(SRCBOARDS)103.o $(SRCBOARDS)106.o $(SRCBOARDS)108.o $(SRCBOARDS)112.o $(SRCBOARDS)117.o \
$(SRCBOARDS)120.o $(SRCBOARDS)121.o $(SRCBOARDS)164.o $(SRCBOARDS)175.o $(SRCBOARDS)176.o $(SRCBOARDS)177.o \
$(SRCBOARDS)178.o $(SRCBOARDS)179.o $(SRCBOARDS)183.o $(SRCBOARDS)185.o $(SRCBOARDS)186.o $(SRCBOARDS)187.o \
$(SRCBOARDS)189.o $(SRCBOARDS)199.o $(SRCBOARDS)208.o $(SRCBOARDS)222.o $(SRCBOARDS)235.o $(SRCBOARDS)253.o \
$(SRCBOARDS)603-5052.o $(SRCBOARDS)8157.o $(SRCBOARDS)8237.o $(SRCBOARDS)411120-c.o $(SRCBOARDS)830118C.o $(SRCBOARDS)a9711.o \
$(SRCBOARDS)a9746.o $(SRCBOARDS)addrlatch.o $(SRCBOARDS)ax5705.o $(SRCBOARDS)bandai.o $(SRCBOARDS)bmc13in1jy110.o \
$(SRCBOARDS)bmc42in1r.o $(SRCBOARDS)bmc64in1nr.o $(SRCBOARDS)bmc70in1.o $(SRCBOARDS)bonza.o $(SRCBOARDS)bs-5.o $(SRCBOARDS)copyfami_mmc3.o \
$(SRCBOARDS)dance.o $(SRCBOARDS)datalatch.o $(SRCBOARDS)deirom.o $(SRCBOARDS)dream.o $(SRCBOARDS)__dummy_mapper.o $(SRCBOARDS)edu2000.o $(SRCBOARDS)fk23c.o \
$(SRCBOARDS)ghostbusters63in1.o $(SRCBOARDS)gs-2004.o $(SRCBOARDS)gs-2013.o $(SRCBOARDS)h2288.o $(SRCBOARDS)karaoke.o $(SRCBOARDS)kof97.o \
$(SRCBOARDS)konami-qtai.o $(SRCBOARDS)ks7032.o $(SRCBOARDS)malee.o $(SRCBOARDS)mmc1.o $(SRCBOARDS)mmc3.o \
$(SRCBOARDS)mmc5.o $(SRCBOARDS)n106.o $(SRCBOARDS)n625092.o $(SRCBOARDS)n-c22m.o $(SRCBOARDS)novel.o $(SRCBOARDS)sachen.o \
$(SRCBOARDS)sc-127.o $(SRCBOARDS)sheroes.o $(SRCBOARDS)sl1632.o $(SRCBOARDS)smb2j.o $(SRCBOARDS)subor.o $(SRCBOARDS)super24.o \
$(SRCBOARDS)supervision.o $(SRCBOARDS)t-227-1.o $(SRCBOARDS)t-262.o $(SRCBOARDS)tengen.o $(SRCBOARDS)tf-1201.o \
$(SRCINPUT)arkanoid.o $(SRCINPUT)bworld.o $(SRCINPUT)cursor.o $(SRCINPUT)fkb.o $(SRCINPUT)ftrainer.o $(SRCINPUT)hypershot.o \
$(SRCINPUT)mahjong.o $(SRCINPUT)mouse.o $(SRCINPUT)oekakids.o $(SRCINPUT)powerpad.o $(SRCINPUT)quiz.o $(SRCINPUT)shadow.o \
$(SRCINPUT)suborkb.o $(SRCINPUT)toprider.o $(SRCINPUT)zapper.o \
$(SRCMAPPER)6.o $(SRCMAPPER)8.o $(SRCMAPPER)16.o $(SRCMAPPER)17.o $(SRCMAPPER)18.o $(SRCMAPPER)21.o $(SRCMAPPER)22.o \
$(SRCMAPPER)24and26.o $(SRCMAPPER)25.o $(SRCMAPPER)27.o $(SRCMAPPER)32.o $(SRCMAPPER)33.o $(SRCMAPPER)40.o $(SRCMAPPER)41.o \
$(SRCMAPPER)42.o $(SRCMAPPER)46.o $(SRCMAPPER)50.o $(SRCMAPPER)51.o $(SRCMAPPER)59.o $(SRCMAPPER)60.o $(SRCMAPPER)61.o \
$(SRCMAPPER)62.o $(SRCMAPPER)65.o $(SRCMAPPER)67.o $(SRCMAPPER)69.o $(SRCMAPPER)71.o $(SRCMAPPER)72.o $(SRCMAPPER)73.o \
$(SRCMAPPER)75.o $(SRCMAPPER)76.o $(SRCMAPPER)77.o $(SRCMAPPER)79.o $(SRCMAPPER)80.o $(SRCMAPPER)82.o $(SRCMAPPER)83.o \
$(SRCMAPPER)85.o $(SRCMAPPER)86.o $(SRCMAPPER)89.o $(SRCMAPPER)91.o $(SRCMAPPER)92.o $(SRCMAPPER)97.o $(SRCMAPPER)99.o \
$(SRCMAPPER)151.o $(SRCMAPPER)193.o $(SRCMAPPER)201.o $(SRCMAPPER)202.o $(SRCMAPPER)203.o $(SRCMAPPER)204.o $(SRCMAPPER)212.o \
$(SRCMAPPER)213.o $(SRCMAPPER)214.o $(SRCMAPPER)215.o $(SRCMAPPER)217.o $(SRCMAPPER)225.o $(SRCMAPPER)227.o $(SRCMAPPER)228.o \
$(SRCMAPPER)229.o $(SRCMAPPER)230.o $(SRCMAPPER)231.o $(SRCMAPPER)232.o $(SRCMAPPER)234.o $(SRCMAPPER)241.o $(SRCMAPPER)242.o \
$(SRCMAPPER)244.o $(SRCMAPPER)246.o $(SRCMAPPER)255.o $(SRCMAPPER)emu2413.o $(SRCMAPPER)mmc2and4.o $(SRCMAPPER)simple.o \
$(SRCUTILS)ConvertUTF.o $(SRCUTILS)crc32.o $(SRCUTILS)endian.o $(SRCUTILS)general.o $(SRCUTILS)guid.o \
$(SRCUTILS)md5.o $(SRCUTILS)memory.o $(SRCUTILS)unzip.o $(SRCUTILS)xstring.o \
$(SRC)asm.o $(SRC)cart.o $(SRC)cheat.o $(SRC)conddebug.cpp $(SRC)config.o $(SRC)debug.o $(SRC)drawing.o $(SRC)emufile.o $(SRC)fceu.o \
$(SRC)fds.o $(SRC)file.o $(SRC)filter.o $(SRC)ines.o $(SRC)input.o $(SRC)movie.o $(SRC)netplay.o $(SRC)nsf.o \
$(SRC)oldmovie.o $(SRC)palette.o $(SRC)ppu.o $(SRC)sound.o $(SRC)state.o $(SRC)unif.o $(SRC)video.o $(SRC)vsuni.o \
$(SRC)wave.o $(SRC)x6502.o \
$(COM_DRV)args.o $(COM_DRV)cheat.o $(COM_DRV)config.o $(COM_DRV)configSys.o $(COM_DRV)hq2x.o $(COM_DRV)hq3x.o \
$(COM_DRV)nes_ntsc.o $(COM_DRV)scale2x.o $(COM_DRV)scale3x.o $(COM_DRV)scalebit.o $(COM_DRV)vidblit.o \
$(CAANOO_DRV)config.o $(CAANOO_DRV)gui.o $(CAANOO_DRV)input.o $(CAANOO_DRV)joystick.o $(CAANOO_DRV)video.o $(CAANOO_DRV)main.o \
$(CAANOO_DRV)sound.o $(CAANOO_DRV)throttle.o $(CAANOO_DRV)unix-netplay.o

all : $(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -o ./build/$(TARGET) $(OBJS) $(LFLAGS) && $(STRIP) ./build/$(TARGET)
	
.c.o:
	$(CC) $(INCS) $(CFLAGS) $(DEFS) -c $< -o $@

.cpp.o:
	$(CXX) $(INCS) $(CFLAGS) $(DEFS) -c $< -o $@
	
clean: 
	rm -f ./build/*.gpe $(SRC)*.o $(SRCBOARDS)*.o $(SRCINPUT)*.o $(SRCMAPPER)*.o $(SRCUTILS)*.o $(CAANOO_DRV)*.o $(COM_DRV)*.o