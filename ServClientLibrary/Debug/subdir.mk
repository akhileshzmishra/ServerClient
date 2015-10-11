################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../ConfFileManager.cpp \
../DataInterpreter.cpp \
../JobScheduler.cpp \
../LibraryInterface.cpp \
../Main.cpp \
../Scheduler.cpp \
../ServClientCommDI.cpp \
../SocketSession.cpp \
../SocketUtility.cpp \
../SystemConfig.cpp \
../XLAcceptorThread.cpp \
../XLAlarm.cpp \
../XLBaseSocket.cpp \
../XLClient.cpp \
../XLClientThread.cpp \
../XLCommonRecieverThread.cpp \
../XLEventQueue.cpp \
../XLEventThread.cpp \
../XLListeningThread.cpp \
../XLMultiAlarm.cpp \
../XLMutex.cpp \
../XLObject.cpp \
../XLSSLSocket.cpp \
../XLSemaPhore.cpp \
../XLServer.cpp \
../XLServerClientHandler.cpp \
../XLServingThread.cpp \
../XLSimpleThreads.cpp \
../XLSocket.cpp \
../XLSocketFactory.cpp \
../XLThreadPool.cpp \
../XLThreadUtility.cpp 

OBJS += \
./ConfFileManager.o \
./DataInterpreter.o \
./JobScheduler.o \
./LibraryInterface.o \
./Main.o \
./Scheduler.o \
./ServClientCommDI.o \
./SocketSession.o \
./SocketUtility.o \
./SystemConfig.o \
./XLAcceptorThread.o \
./XLAlarm.o \
./XLBaseSocket.o \
./XLClient.o \
./XLClientThread.o \
./XLCommonRecieverThread.o \
./XLEventQueue.o \
./XLEventThread.o \
./XLListeningThread.o \
./XLMultiAlarm.o \
./XLMutex.o \
./XLObject.o \
./XLSSLSocket.o \
./XLSemaPhore.o \
./XLServer.o \
./XLServerClientHandler.o \
./XLServingThread.o \
./XLSimpleThreads.o \
./XLSocket.o \
./XLSocketFactory.o \
./XLThreadPool.o \
./XLThreadUtility.o 

CPP_DEPS += \
./ConfFileManager.d \
./DataInterpreter.d \
./JobScheduler.d \
./LibraryInterface.d \
./Main.d \
./Scheduler.d \
./ServClientCommDI.d \
./SocketSession.d \
./SocketUtility.d \
./SystemConfig.d \
./XLAcceptorThread.d \
./XLAlarm.d \
./XLBaseSocket.d \
./XLClient.d \
./XLClientThread.d \
./XLCommonRecieverThread.d \
./XLEventQueue.d \
./XLEventThread.d \
./XLListeningThread.d \
./XLMultiAlarm.d \
./XLMutex.d \
./XLObject.d \
./XLSSLSocket.d \
./XLSemaPhore.d \
./XLServer.d \
./XLServerClientHandler.d \
./XLServingThread.d \
./XLSimpleThreads.d \
./XLSocket.d \
./XLSocketFactory.d \
./XLThreadPool.d \
./XLThreadUtility.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -pthread -D__UNIX__ -D__MAX_OS_ -I/usr/lib/ -I/usr/include/openssl -I../openssl/include/openssl -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


