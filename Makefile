#CC = cc
#CFLAGS
#CPPFLAGS

CXX = g++
CXXFLAGS = -IC:\opencv\build\include -LC:\opencv\build\lib -llibopencv_calib3d480 -llibopencv_core480 -llibopencv_dnn480 -llibopencv_features2d480 -llibopencv_flann480 -llibopencv_gapi480 -llibopencv_highgui480 -llibopencv_imgcodecs480 -llibopencv_imgproc480 -llibopencv_ml480 -llibopencv_objdetect480 -llibopencv_photo480 -llibopencv_stitching480 -llibopencv_video480 -llibopencv_videoio480 -lgdi32 -lws2_32 -I 'C:\Program Files (x86)\wxWidgets\include' -I 'C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll\mswu' -L 'C:\Program Files (x86)\wxWidgets\lib\gcc_x64_dll'-mwindows -l wxmsw32u_core -l wxbase32u -l wxmsw32u_adv


CLIENT_TARGET_EXEC = client.exe
SERVER_TARGET_EXEC = server.exe

BUILD_DIR = ./build
SRC_DIR = ./src

CLIENT_SRC_DIR = ./src/Client
SERVER_SRC_DIR = ./src/Server
PNET_SRC_DIR = ./src/PNet
UI_SRC_DIR = ./src/UI

all: $(BUILD_DIR)/$(CLIENT_TARGET_EXEC) $(BUILD_DIR)/$(SERVER_TARGET_EXEC)

#Get-Childitem -Path "./src/PNet" -filter "*.cpp" -recurse | ForEach-Object { $_.FullName -replace "^.*\\src\\PNet\\","/src/PNet/" }
#Get-Childitem -Path "./src/PNet", "./src/Client" -filter "*.cpp" -recurse | ForEach-Object { $_.FullName -replace "^.*\\src\\PNet\\","src/PNet/" -replace "^.*\\src\\Client\\","src/Client/" -replace ".cpp",""})

ifeq ($(OS),Windows_NT)
    SHELL = powershell.exe
	.SHELLFLAGS = -NoProfile -Command 

    CLIENT_SRCS = $(shell Get-Childitem -Path "./src/Client" -filter "*.cpp" -recurse | ForEach-Object { $$_.FullName -replace "^.*\\src\\Client\\","src/Client/" -replace ".cpp",""})
    SERVER_SRCS = $(shell Get-Childitem -Path "./src/Server" -filter "*.cpp" -recurse | ForEach-Object { $$_.FullName -replace "^.*\\src\\Server\\","src/Server/" -replace ".cpp",""})
	PNET_SRCS = $(shell Get-Childitem -Path "./src/PNet" -filter "*.cpp" -recurse | ForEach-Object { $$_.FullName -replace "^.*\\src\\PNet\\","src/PNet/" -replace ".cpp",""})
	UI_SRCS = $(shell Get-Childitem -Path "./src/UI" -filter "*.cpp" -recurse | ForEach-Object { $$_.FullName -replace "^.*\\src\\UI\\","src/UI/" -replace ".cpp",""})

	CLIENT_OBJS = $(CLIENT_SRCS:%=$(BUILD_DIR)/%.o)
	SERVER_OBJS = $(SERVER_SRCS:%=$(BUILD_DIR)/%.o)
	PNET_OBJS = $(PNET_SRCS:%=$(BUILD_DIR)/%.o)
	UI_OBJS = $(UI_SRCS:%=$(BUILD_DIR)/%.o)

   all: $(BUILD_DIR)/$(CLIENT_TARGET_EXEC) $(BUILD_DIR)/$(SERVER_TARGET_EXEC)

    $(BUILD_DIR)/$(CLIENT_TARGET_EXEC): $(CLIENT_OBJS) $(PNET_OBJS)
		$(CXX) $^ -o $@ $(CXXFLAGS)

    $(BUILD_DIR)/$(SERVER_TARGET_EXEC): $(SERVER_OBJS) $(PNET_OBJS) 
		$(CXX) $^ -o $@ $(CXXFLAGS)
	
    $(BUILD_DIR)/%.o: %.cpp %.h
		$(CXX) -c $< -o $@ $(CXXFLAGS)
	

else

endif

.PHONY: clean run-client run-server

clean:
	echo "clean"
	echo $(CLIENT_SRCS)
	rm -f -r $(BUILD_DIR)/$(SRC_DIR)

run-client:
	$(BUILD_DIR)/$(CLIENT_TARGET_EXEC)

run-server:
	$(BUILD_DIR)/$(SERVER_TARGET_EXEC)