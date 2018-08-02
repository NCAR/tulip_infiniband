DOCKER_TAG      ?= tulip
DEBRELEASE	?= stretch
BUILD_ARGS      ?= --rm 
# Optional user-defined variable declaring which network interface to use for Mac
ETHDEV		:=

# Determine operating system
ifeq ($(shell uname),Linux)
	OS=Linux
else ifeq ($(shell uname),Darwin)
	OS=Mac
else
$(error Error, only compatable with Linux and Mac)
endif

# For Mac, determine which network interface to use, unless specified by user
ifeq ($(OS),Mac)
	ifndef ETHDEV
		ifneq ($(shell ipconfig getifaddr en0),)
			ETHDEV=en0
		else ifneq ($(shell ipconfig getifaddr en1),)
			ETHDEV=en1
		else ifneq ($(shell ipconfig getifaddr en2),)
			ETHDEV=en2
		else ifneq ($(shell ipconfig getifaddr en3),)
			ETHDEV=en3
		endif
	endif
	# Get the ip address
	IP=$(shell ipconfig getifaddr $(ETHDEV))
# Define error message if no ip address can be found
ifeq ($(IP),)
$(error Error, no ip address found)
endif
endif

# Define appropriate arguments for the docker run depending on the operating system.
ifeq ($(OS),Linux)
	RUN_ARGS ?= docker run --network=host --cap-add=SYS_PTRACE --security-opt=apparmor:unconfined \
	-it --log-driver=syslog -h $(DOCKER_TAG) -v /dev/log:/dev/log --name $(DOCKER_TAG) \
	-v /tmp/.X11-unix:/tmp/.X11-unix -v /home:/home -v $(XAUTHORITY):$(XAUTHORITY) \
	-e DISPLAY=unix$(DISPLAY) -e XAUTHORITY=$(XAUTHORITY)
else ifeq ($(OS),Mac)
	RUN_ARGS ?= xhost + $(IP); \
	docker run --network=host --cap-add=SYS_PTRACE --security-opt=apparmor:unconfined \
	-it -h $(DOCKER_TAG) --name $(DOCKER_TAG) \
	-e DISPLAY=$(IP):0 \
	-v /tmp/.X11-unix:/tmp/.X11-unix -v ~:/home
endif

# Define makefile targets
default: run

build:
	docker build $(BUILD_ARGS) \
		--build-arg DEBRELEASE=$(DEBRELEASE) \
		--network=host -t $(DOCKER_TAG) .

stop:
	docker stop $(DOCKER_TAG) || true
	docker rm $(DOCKER_TAG) || true

nocache:
	$(eval BUILD_ARGS := $(BUILD_ARGS) --no-cache)

clean: nocache build

run: build stop
	$(RUN_ARGS) -d --restart unless-stopped $(DOCKER_TAG)

debug: build stop
	$(RUN_ARGS) --rm $(DOCKER_TAG)

bash: build stop
	$(RUN_ARGS) --rm $(DOCKER_TAG) /bin/bash

help: 
	@echo Execute '"make <target>"' for specific options within the makefile. Targets are described below. The default is run. If a specific network interface is desired, define ETHDEV in the make call. For example: make ETHDEV=en1 debug.
	@echo run:    "\t"  Builds and runs Tulip. Restarts the GUI until process is stopped or the window is closed with the close button
	@echo debug:  "\t"  Builds and runs Tulip while forwarding any standard output to the terminal
	@echo bash:   "\t"  Builds the Tulip container but does not run the GUI. User is placed in the container in the terminal
	@echo build:  "\t"  Builds the Tulip container
	@echo stop:   "\t"  Stops the Tulip container and removes the container files from the host system
	@echo clean:  "\t"  Builds and runs Tulip from scratch. Does not use the cache. This takes a long time

