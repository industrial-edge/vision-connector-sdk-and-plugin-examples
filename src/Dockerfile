ARG BASE_IMAGE=debian:bullseye-20221219-slim
ARG COMMON_PATH="/cmake-3.21.7-linux-x86_64/bin/:/clangd_language_server/clangd_16.0.2/bin/"
ARG COMMON_LIB_PATH="/usr/local/lib/"


FROM mcr.microsoft.com/vscode/devcontainers/cpp:bullseye AS dev_container
ARG COMMON_PATH
ARG COMMON_LIB_PATH
ENV PATH=${COMMON_PATH}:${PATH}
ENV LIBRARY_PATH=${COMMON_LIB_PATH}:${LIBRARY_PATH}
ENV LD_LIBRARY_PATH=${COMMON_LIB_PATH}:${LD_LIBRARY_PATH}
RUN apt update
RUN apt install -y git git-lfs
COPY requirements ./requirements
RUN chmod -R +x ./requirements
RUN ./requirements/install_build_requirements.sh
RUN ./requirements/install_opencv.sh
RUN ./requirements/install_test_requirements.sh
RUN ./requirements/install_clangd_language_server_16_0_2.sh


FROM $BASE_IMAGE AS build_img
ARG COMMON_PATH
ARG COMMON_LIB_PATH
ENV PATH=${COMMON_PATH}:${PATH}
ENV LIBRARY_PATH=${COMMON_LIB_PATH}:${LIBRARY_PATH}
ENV LD_LIBRARY_PATH=${COMMON_LIB_PATH}:${LD_LIBRARY_PATH}
WORKDIR /vca-drivers
COPY requirements ./requirements
RUN chmod -R +x ./requirements
RUN ./requirements/install_build_requirements.sh
RUN ./requirements/install_opencv.sh


FROM build_img AS test_img
RUN ./requirements/install_test_requirements.sh


FROM build_img AS local_build_img
WORKDIR /vca-drivers
COPY sdk ./sdk
COPY drivers ./drivers
COPY CMakeLists.txt CMakeLists.txt
COPY CMakePresets.json CMakePresets.json
COPY build_drivers.sh build_drivers.sh
RUN chmod +x build_drivers.sh