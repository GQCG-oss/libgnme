FROM ubuntu:22.04

# non interactive frontend for locales
ARG DEBIAN_FRONTEND=noninteractive

# installing utils
RUN apt-get update \
    && apt-get install -y --no-install-recommends \ 
        build-essential \
        wget \
        make \
        git \
        procps \
        locales \
        curl \
        openssh-client \
        python3-pip \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/* \
    && apt-get autoremove -y

# install miniconda
ENV PATH="/usr/local/miniconda3/bin:${PATH}"
ARG PATH="/usr/local/miniconda3/bin:${PATH}"
RUN wget \
    https://repo.anaconda.com/miniconda/Miniconda3-py38_4.12.0-Linux-x86_64.sh \
    && bash Miniconda3-py38_4.12.0-Linux-x86_64.sh -p /usr/local/miniconda3 -b \
    && rm -f Miniconda3-py38_4.12.0-Linux-x86_64.sh 
RUN conda --version

# install required conda packages
RUN conda install -c conda-forge cmake mkl intel-openmp doxygen

RUN ldconfig

ENTRYPOINT bash
