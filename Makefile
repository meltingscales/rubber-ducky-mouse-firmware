# Makefile: Docker-only build for Rubber Ducky Mark I firmware

IMAGE = ducky-avr32

.PHONY: all firmware clean image shell

# Host targets
all: image
	docker run --rm -v $(CURDIR):/work $(IMAGE) make firmware

clean:
	docker run --rm -v $(CURDIR):/work $(IMAGE) make clean || rm -rf build

image:
	docker build -t $(IMAGE) .

shell: image
	docker run --rm -it -v $(CURDIR):/work $(IMAGE) /bin/bash

# Container target
firmware:
	@echo "[IN DOCKER] Building firmware..."
	make -f Makefile.docker
	fi
	@echo "[IN DOCKER] Building firmware with legacy toolchain..."
	make -f Makefile.legacy
