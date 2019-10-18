FROM ubuntu:14.04

RUN apt-get update -y && apt-get install -y build-essential \
	nano \
	man \
	openssh-server

RUN groupadd mpi_user

RUN useradd mpi_user -g mpi_user -m -s /bin/bash

ADD openmpi-2.0.2.tar.gz /opt/

RUN cd /opt && chown -hR mpi_user:mpi_user openmpi-2.0.2

RUN mkdir -p /var/run/sshd

RUN echo "mpi_user ALL=(ALL:ALL) NOPASSWD:ALL" | (EDITOR="tee -a" visudo)

RUN echo "mpi_user:mpi" | chpasswd

USER mpi_user

RUN cd /opt/openmpi-2.0.2 && ./configure --prefix=/opt/openmpi-2.0.2 -with-sge && make all install

ENV PATH="/opt/openmpi-2.0.2/bin:$PATH"

ENV LD_LIBRARY_PATH="/opt/openmpi-2.0.2/lib:LD_LIBRARY_PATH"


#docker build -t openmpi_mno_2017/openmpi:v1 .


#docker run -dit -v $(pwd):/results -p 22 -h master --name master_container openmpi_mno_2017/openmpi:v1 /bin/bash

#docker $(docker-machine config default) run -dit -v $(pwd):/results -p 22 -h master --name master_container openmpi_mno_2017/openmpi:v1 /bin/bash



