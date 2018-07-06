DOCKER_TAG      ?= tulip
DEBRELEASE      ?= stretch
BUILD_ARGS      ?= --rm 
UNAME = $(shell uname)

ifeq ($(UNAME),Linux)
	RUN_ARGS ?= docker run --network=host --cap-add=SYS_PTRACE --security-opt=apparmor:unconfined \
	-it --log-driver=syslog -h $(DOCKER_TAG) -v /dev/log:/dev/log --name $(DOCKER_TAG) \
	-v /tmp/.X11-unix:/tmp/.X11-unix -v /home:/home -v $(XAUTHORITY):$(XAUTHORITY) \
	-e DISPLAY=unix$(DISPLAY) -e XAUTHORITY=$(XAUTHORITY)
else ifeq ($(UNAME),Darwin)
	RUN_ARGS ?= xhost + $(shell ifconfig en0 | grep inet | awk '$$1=="inet" {print $$2}'); \
	docker run --network=host --cap-add=SYS_PTRACE --security-opt=apparmor:unconfined \
	-it -h $(DOCKER_TAG) --name $(DOCKER_TAG) \
	-e DISPLAY=$(shell ifconfig en0 | grep inet | awk '$$1=="inet" {print $$2}'):0 \
	-v /tmp/.X11-unix:/tmp/.X11-unix
endif

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
