DOCKER_RUN = docker run --rm -it
DOCKER_BUILD = docker build -t
DOCKER_NETWORK = docker network create -d bridge
VOLUMES = -v "$$PWD:/usr/src/app"
WORKDIR = -w /usr/src/app
NETWORK = --net myslack
IMAGE = myslack
NUMBER = 0

start_server_container :
	$(DOCKER_RUN) $(VOLUMES) $(WORKDIR) $(NETWORK) --name server $(IMAGE)

start_client_container :
	$(DOCKER_RUN) $(VOLUMES) $(WORKDIR) $(NETWORK) --name client_$(NUMBER) $(IMAGE)

build :
	$(DOCKER_BUILD) $(IMAGE) .

create_network :
	$(DOCKER_NETWORK) $(IMAGE)
